#include "TcpConnection.h"
#include "EventLoop.h"
#include "Socket.h"
#include "Channel.h"
#include "SocketOps.h"

#include <errno.h>
#include <stdio.h>

//写TcpConnection的同时倒逼如何设计Bufffer
//为什么没有考虑线程安全？
//所有函数的执行都在EventLoop所属的线程中执行
//如果state() == kDisconnecting_说明服务器调用了shutdown函数希望主动关闭连接
//还没有仔细考虑可能出现的问题

TcpConnection::TcpConnection(EventLoop *loop, int sockfd,
                            const InetAddress &localAddr,
                            const InetAddress &peerAddr)
    :   loop_(loop),
        channel_(new Channel(loop, sockfd)),
        socket_(new Socket(sockfd)),
        localAddr_(localAddr),
        peerAddr_(peerAddr),
        state_(kConnecting)
{
    channel_->setReadCallback(std::bind(&TcpConnection::handleRead, this, _1));
    channel_->setWriteCallback(std::bind(&TcpConnection::handleWrite, this));
    socket_->setNonBlocking(true);
    socket_->setTcpNoDelay(true);
}

//do something ???
TcpConnection::~TcpConnection()
{
    assert(state() == kDisconnected);
    printf("TcpConnection::~TcpConnection peerIp = %s, peerPort=%s\n",
            peerAddr_.ipToString().c_str(),
            peerAddr_.portToString().c_str());
}

void TcpConnection::send(const string &data)
{
    send(data.c_str(), data.size());
}

void TcpConnection::send(const char *data, size_t len)
{
    if(loop_->isInLoopThread()) 
    {
        sendInLoop(data, len);
    }
    else    
    {
        loop_->runInLoop(std::bind(&TcpConnection::sendInLoop, shared_from_this(), data, len));
    }
}

void TcpConnection::sendInLoop(const char *data, size_t len)
{   
    loop_->assertInLoopThread();

    if(state() != kConnected)
        return;
    //直接发送,只写一次，因为是非阻塞IO
    bool writeFault = false;
    size_t writtenBytes = 0;
    if(outputBuffer_.empty() && !channel_->isWriting())
    {
        ssize_t n = sockets::write(socket_->fd(), data, len);
        if(n == -1) 
        {
            if(errno != EAGAIN && errno != EWOULDBLOCK)
            {
                writeFault = true;
            }
        }
        else
        {
            writtenBytes = n;
        }
    } 

    //如果writeFault为true，如何处理？？？？？
    if(!writeFault && writtenBytes < len)
    {
        outputBuffer_.append(data + writtenBytes, len - writtenBytes);
        channel_->enableWrite();        //关注可写事件
    }
}

void TcpConnection::shutdown()
{
    loop_->runInLoop(
                    std::bind(&TcpConnection::shutdownInLoop, shared_from_this()));
}

void TcpConnection::shutdownInLoop()
{
    loop_->assertInLoopThread();

    //输出缓冲区没有数据，可以关闭，否则等待直到缓冲区为空
    //设置连接状态为kDisconnecting, 这样当handleWrite将数据写完之后发现如果处于
    //kDisconnecting状态，则再次调用shutdownInLoop，这样就可以进行半关闭
    if(state() == kConnected)
        setState(kDisconnecting);
    if(!channel_->isWriting())
        socket_->shutdownWrite();
}

void TcpConnection::connectEstablished()
{
    loop_->assertInLoopThread();

    assert(state() == kConnecting);
    setState(kConnected);
    channel_->tie(shared_from_this());
    channel_->enableRead();

    newConnectionCallback_(shared_from_this());
}

void TcpConnection::connectDestroyed()
{
//    printf("use_count[6] = %ld\n", shared_from_this().use_count());
    assert(state() == kDisconnected || state() == kDisconnecting);
    setState(kDisconnected);
    channel_->disableAll();
    channel_->remove();
}

//do something else ????
void TcpConnection::handleRead(Timestamp recieveTime)
{
    loop_->assertInLoopThread();

    int savedErrno = 0;
    ssize_t n = inputBuffer_.readFd(socket_->fd(), &savedErrno);
    if(n > 0) 
    {
        messageCallback_(shared_from_this(), &inputBuffer_ , recieveTime);
    }
    else  if(n == 0)
    {   
        handleClose();
    }
    else   
    {
        handleError();
    }
}

void TcpConnection::handleWrite()
{
    loop_->assertInLoopThread();

    ssize_t writtenBytes = sockets::write(socket_->fd(), outputBuffer_.readerBegin(), outputBuffer_.readableBytes());
    outputBuffer_.retrieve(writtenBytes);

    if(outputBuffer_.empty())
    {
        channel_->disableWrite();
        if(state() == kDisconnecting)
            shutdownInLoop();
    }
}


void TcpConnection::handleClose()
{
    loop_->assertInLoopThread();

    setState(kDisconnected);
    //closeCallback_(shared_from_this())会导致coredump。
    TcpConnectionPtr guard(shared_from_this());
    //printf("use_count[1] = %ld\n", guard.use_count());
    closeCallback_(guard);
    //printf("use_count[5] = %ld\n", guard.use_count());
}

void TcpConnection::handleError()
{
    //do something....
}