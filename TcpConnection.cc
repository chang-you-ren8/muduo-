#include "TcpConnection.h"
#include "EventLoop.h"
#include "Channel.h"
#include "Socket.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>

#include <functional>

TcpConnection::TcpConnection(EventLoop *loop,
                            int sockfd,
                            const InetAddress &localAddr,
                            const InetAddress &peerAddr)
    :   loop_(loop),
        channel_(new Channel(loop, sockfd)),
        socket_(new Socket(sockfd)),
        state_(kConnecting),
        localAddr_(localAddr),
        peerAddr_(peerAddr)
{
    assert(loop != NULL);
    //shared_from_this ??
    channel_->setReadCallback(std::bind(&TcpConnection::handleRead, this, _1));
    channel_->setWriteCallback(std::bind(&TcpConnection::handleWrite, this));
    channel_->setCloseCallback(std::bind(&TcpConnection::handleClose, this));
    channel_->setErrorCallback(std::bind(&TcpConnection::handleError, this));

    loop_->runInLoop(std::bind(&TcpConnection::connectEstablished, this));
}

//现在不知道如何处理
TcpConnection::~TcpConnection() = default;

//最主要的是send, shutdown, handleRead, handleClose, handleWrite, handleError的实现
void TcpConnection::send(const char *data, size_t len)
{
    if(loop_->isInLoopThread())
        sendInLoop(data, len);
    else   
        loop_->runInLoop(std::bind(&TcpConnection::sendInLoop, this, data, len)); //shared_from_this????
}

void TcpConnection::send(const std::string &data)
{
    send(data.c_str(), data.size());
}

void TcpConnection::sendInLoop(const char *data, size_t len)
{
    loop_->assertInLoopThread();
    ssize_t nwrote = 0;
    size_t remaining = len;
    bool faultError = false;

    if(state_ == kDisconnected)
    {
        fprintf(stdout, "%s\n", "disconnected, give up writing");
        return;
    }
    //直接写(这些sockfd都是非阻塞IO，所以只写一次)
    if(!channel_->isWriting() && outputBuffer_.empty())
    {
        nwrote = socket::write(channel_->fd(), data, len);
        if(nwrote >= 0)
        {
            remaining -= nwrote;
            //writeCompleteCallback
        }
        else
        {
            nwrote = 0;
            //非阻塞IO写入失败如果返回EWOULDBLOCK或者EAGAIN那么不用处理这种错误
            //否则要处理
            if(errno != EWOULDBLOCK || errno != EAGAIN)
            {
                //use Logger
                fprintf(stderr, "%s\n", "TcpConnection::sendInLoop write error");
                //EPIPE对方关闭读，如果再往其中写入就会产生EPIPE，都是产生SIGPIPE信号
                if(errno == EPIPE || errno == ECONNRESET)
                {
                    faultError = true;
                }
            }
        }
    }

    assert(remaining <= len);
    //faultError表示对连接失效了，不用再关注可写事件了
    //关注可写事件,将剩余数据添加到outputBuffer_的末尾
    if(!faultError && remaining > 0)
    {
        //highWaterMarkCallback_???
        outputBuffer_.append(data + nwrote, remaining);
        //关注可写事件
        if(!channel_->isWriting())
            channel_->enableWrite();
    }
}

void TcpConnection::shutdown()
{
    if(state_ == kConnected)
    {
        //kDisconnecting表明outputBuffer_中可能有待发送数据
        setState(kDisconnecting);
        //shared_from_this ??
        loop_->runInLoop(std::bind(TcpConnection::shutdownInLoop, this));
    }
}

//shutdownWrite只会被这个函数调用
void TcpConnection::shutdownInLoop()
{
    loop_->assertInLoopThread();
    //如果此时没有待发送的数据，直接关闭套接字
    if(!channel_->isWriting())
        socket_>shutdownWrite(); //半关闭
    //那么如果channel_->isWriting()为true,那么什么时候调用shutdownWrite()???
    //当handleWrite()将outputBuffer_写完之后，发现是处于kDisconnecting状态，那么handleWrite将channel_->isWriting设置为false
    //并且再次回调shutdownInLoop
}

//这个函数一定是在loop_所在线程被调用
void TcpConnectoin::handleRead(Timestamp receiveTime)
{
    loop_->assertInLoopThread();

    int savedErrno = 0;
    ssize_t n = inputBuffer_.readFd(channel_->fd(), &savedErrno);
    if(n > 0)
    {
        //这个messageCallback_是库的用户写的，
        //通过将messageCallback_设置为TcpServer的成员，然后再设置为TcpConnection的成员
        messageCallback_(shared_from_this(), &inputBuffer_, receiveTime);
    }
    else if(n == 0)
    {
        //对等方关闭
        handleClose();
    }
    else  
    {
        //use Logger??
        char msg[128];
        fprintf(stderr, "%s\n", strerror_r(savedErrno, msg, sizeof(msg)));
        handleError();
    }
}

void TcpConnection::handleWrite()
{
    loop_->assertInLoopThread();

    if(channel_->isWriting())
    {
        ssize_t n = sockets::write(channel_->fd(),
                                outputBuffer_.peek(),
                                outputBuffer_.readableBytes());
        if(n > 0)
        {
            outputBuffer_.retrieve(n);
            if(outputBuffer_.empty())
            {
                //避免busy loop
                channel_->disableWrite();
                //writeCompleteCallback_
                //服务器端主动调用shutdown
                if(state == kDisconnecting)
                    shutdownInLoop();
            }
        }
        else 
        {
            //use Logger
            fprintf(stderr, "TcpConnection::handleWrite error\n");
        }
    }
    else  
    {
        //什么情况下被调用？？？
        fprintf(stderr, "TcpConnection:handleWrite %d is down, no more writing");
    }
}

//read返回0，表明对方关闭连接
//这个连接没有存在的必要了
void TcpConnection::handleClose()
{
    loop_->assertInLoopThread();

    assert(state_ == kConnected || state_ == kDisconnecting);
    setState(kDisconnected);
    //这个时候channel_为什么不直接从loop_中移除
    channel_->disableAll();

    //使用shared_ptr放置在handleClose结束之前，TcpConnection对象被释放 
    TcpConnectionPtr guardThis(shared_from_this());
    //这个closeCallback_是TcpServer::removeConnection(将TcpConnectionPtr从TcpServer::connections_中删除)
    closeCallback_(guardThis);
    //use Logger???
    fprintf(stdout, "TcpConnection destroyed: fd = %d, localAddress = %s, peerAddress = %s\n",
            channel_->fd(), 
            localAddr_.toString(), 
            peerAddr_.toString());
}


void TcpConnection::handleError()
{
    //读取套接字上的错误
}