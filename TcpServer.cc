#include "TcpServer.h"
#include "EventLoop.h"
#include "Acceptor.h"
#include "SocketOps.h"

TcpServer::TcpServer(EventLoop *loop,
                    const InetAddress &listenAddr,
                    string name)
    :   baseLoop_(loop),
        listenAddr_(listenAddr),
        name_(name),
        pool_(loop),
        acceptor_(new Acceptor(loop, listenAddr)),
        started_(false)
{
    acceptor_->setNewConnectionCallback(std::bind(&TcpServer::newConnection, this, _1, _2));
}


//FixedMe
TcpServer::~TcpServer() 
{

}

//可以多次调用，但是只能在baseLoop_所属的线程调用
void TcpServer::start()
{
    if(!started_)
    {
        started_ = true;
        acceptor_->listen();
        pool_.start();
    }
}

void TcpServer::newConnection(int sockfd, const InetAddress &peerAddr)
{
    InetAddress localAddr = sockets::getLocalAddr(sockfd);
    EventLoop *ioLoop = pool_.getNextLoop();

    TcpConnectionPtr conn(new TcpConnection(ioLoop, sockfd, localAddr, peerAddr));
    connections_.insert(conn);
    conn->setNewConnectionCallback(newConnectionCallback_);
    conn->setMessageCallback(messageCallback_);
    conn->setCloseCallback(std::bind(&TcpServer::removeConnection, this, _1));

    ioLoop->runInLoop(std::bind(&TcpConnection::connectEstablished, conn));
}

void TcpServer::removeConnection(const TcpConnectionPtr &conn)
{
    baseLoop_->runInLoop(std::bind(&TcpServer::removeConnectionInLoop, this, conn));
}

//必须在baseLoop_所在的线程删除，否则就会产生race
void TcpServer::removeConnectionInLoop(const TcpConnectionPtr &conn)
{
    baseLoop_->assertInLoopThread();

    size_t n = connections_.erase(conn);
    assert(n == 1);
    (void)n;

    EventLoop *ioLoop = conn->ownerLoop();
    //must be queueInLoop
    ioLoop->queueInLoop(std::bind(&TcpConnection::connectDestroyed, conn));
}



