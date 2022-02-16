#include "Acceptor.h"
#include "EventLoop.h"
#include "InetAddress.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>

//listenfd为什么设置成nonblocking??
Acceptor::Acceptor(EventLoop *loop, const InetAddress &listenAddr)
    :   loop_(loop),
        acceptSocket_(sockets::createNonblockingOrDie(listenAddr.family())),
        acceptChannel_(loop, acceptSocket_.fd()),
        idleFd_(::open("/dev/null", O_CLOEXEC | O_RDONLY)),
        listening_(false)
{
    assert(loop != NULL);
    assert(idleFd_ >= 0);
    acceptSocket_.bind(listenAddr);
    acceptChannel_.setReadCallback(&Acceptor::handleRead, this);
}

Acceptor::~Acceptor()
{
    //这个accpetChannel_是由Acceptor负责的，所以要在析构函数中释放
    acceptChannel_.disableAll();
    acceptChannel_.removeChannel(); //从loop_中删除
    ::close(idleFd_);
}

//由TcpServer::start() 将这函数注册到EventLoop中，然后再EventLoop::doPendingFunctors()中执行
void Acceptor::listen()
{
    loop_->assertInLoopThread();

    listening_ = true;
    acceptSocket_.listen();
    acceptChannel_.enableRead();
}

//在这里函数里面回调TcpServer::newConnection也就是Acceptor::newConnectionCallback_
void Acceptor::handleRead()
{
    loop_->assertInLoopThread();

    InetAddress peerAddr;
    int connfd = acceptSocket_.accept(&peerAddr);
    if(connfd >= 0)
    {
        //FIX??
        assert(newConnectionCallback_);
        newConnectionCallback_(connfd, peerAddr); 
    }
    else  
    {
        //MORE INFOR??
        fprintf(stderr, "%s\n", "Acceptor::handleRead error");
        if(errno == EMFILE)
        {
            ::close(idleFd_);
            idleFd_ = ::accept(acceptSocket_.fd(), NULL, NULL);
            ::close(idleFd_);
            idleFd_ = ::open("/dev/null", O_RDONLY | O_CLOEXEC);
            assert(idleFd_ >= 0);
        }
    }
}