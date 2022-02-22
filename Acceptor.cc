#include "Acceptor.h"
#include "Socket.h"
#include "Channel.h"
#include "EventLoop.h"
#include "SocketOps.h"
#include "InetAddress.h"

#include <fcntl.h>
#include <errno.h>
#include <assert.h>

Acceptor::Acceptor(EventLoop *loop, const InetAddress &listendAddr)
    :   acceptSocket_(new Socket(sockets::createNonBlockOrDie())),
        acceptChannel_(new Channel(loop, acceptSocket_->fd())),
        idleFd_(::open("/dev/null", O_RDONLY | O_CLOEXEC))
{
    assert(loop != NULL);
    assert(idleFd_ >= 0);
    acceptSocket_->setReuseAddr(true);
    acceptSocket_->bind(listendAddr);
    acceptChannel_->setReadCallback(std::bind(&Acceptor::handleRead, this));
}

Acceptor::~Acceptor()
{
    ::close(idleFd_);
}

void Acceptor::listen()
{
    acceptSocket_->listen();
    acceptChannel_->enableRead();
}


void Acceptor::handleRead()
{
    InetAddress peerAddr;
    int conn = acceptSocket_->accept(&peerAddr);

    if(conn >= 0)
    {
        assert(newConnectionCallback_);
        newConnectionCallback_(conn, peerAddr);
    }
    else     
    {
        if(errno == EMFILE)
        {
            ::close(idleFd_);
            idleFd_ = ::accept(acceptSocket_->fd(), NULL, NULL);
            ::close(idleFd_);
            idleFd_ = ::open("/dev/null", O_RDONLY | O_CLOEXEC);
            //assert(idleFd_) ??? 
        }
    }
}