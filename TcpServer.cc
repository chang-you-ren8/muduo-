#include "TcpServer.h"

#include "TcpConnection.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "Channel.h"
#include "SocketOps.h"

#include <assert.h>

TcpServer::TcpServer(EventLoop *loop,
                    const InetAddress &listenAddr,
                    const std::string &name)
    :   loop_(loop),
        acceptor_(new Acceptor(loop, listenAddr)),
        name_(name),
        ipPort_(listenAddr.portToString()),
        started_(false)
{
    assert(loop != NULL);
    acceptor_->setNewConnectionCallback(std::bind(&TcpServer::newConnection, this, _1, _2));
}

TcpServer::~TcpServer()
{
    //???
}

void TcpServer::start()
{
    assert(!started_);
    started_ = true;
    acceptor_->listen();
}

//被acceptor回调
void TcpServer::newConnection(int sockfd, const InetAddress &peerAddr)
{
    InetAddress localAddr(sockets::getLocalAddr(sockfd));
    TcpConnectionPtr conn(new TcpConnection(loop_, localAddr, peerAddr));

    connections_.insert(conn);
    conn->setMessageCallback(messageCallback_);
    conn->setConnectionCallback(connectoinCallback_);
    conn->setCloseCallback(std::bind(&TcpServer::removeConnection, this, _1));

    loop_->runInLoop(std::bind(&TcpConnection::connectEstablished, conn));
}

void TcpServer::removeConnection(const TcpConnectionPtr &conn)
{
    loop_->runInLoop(&TcpServer::removeConnectionInLoop, this, conn);
}

void TcpServer::removeConnectionInLoop(const TcpConnectionPtr &conn)
{
    loop_->assertInLoopThread();

    size_t n = connections_.erase(conn);
    assert(n == 1);
    (void)n;

    //conn所处于的EventLoop
    EventLoop *ioLoop = conn->getLoop();
    ioLoop->runInLoop(&TcpConnecton::connectDestroyed, conn);
}