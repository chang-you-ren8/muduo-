#include "Socket.h"
#include "SocketOps.h"

#include <assert.h>
#include <fcntl.h>


Socket::Socket(int sockfd)
    :   sockfd_(sockfd)
{

}

Socket::~Socket()
{
    sockets::close(sockfd_);
}


void Socket::bind(const InetAddress &listenAddr)
{
    sockets::bindOrDie(sockfd_, listenAddr);
}

void Socket::listen()
{
    sockets::listenOrDie(sockfd_);
}

int Socket::accept(InetAddress *peerAddr)
{
    return sockets::accept(sockfd_, peerAddr);
}

void Socket::shutdownWrite()
{
    int ret = ::shutdown(sockfd_, SHUT_WR);
    assert(ret == 0);
}

void Socket::setReuseAddr(bool on)
{
    int val = on ? 1 : 0;
    int ret = ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    assert(ret == 0);
}

void Socket::setNonBlocking(bool on)
{
    if(on)
    {
        int oldFlag = ::fcntl(sockfd_, F_GETFL);
        int newFlag = oldFlag | O_NONBLOCK;
        int ret = ::fcntl(sockfd_, F_SETFL, newFlag);
        assert(ret == 0);
    }
}

void Socket::setTcpNoDelay(bool on)
{
}

void Socket::setKeepAlive(bool on)
{

}



