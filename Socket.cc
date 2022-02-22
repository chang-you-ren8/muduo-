#include "Socket.h"
#include "SocketOps.h"

#include <assert.h>


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

void Socket::setReuseAddr(bool on)
{
    int val = on ? 1 : 0;
    int ret = ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    assert(ret == 0);
}

void Socket::setTcpNoDelay(bool on)
{

}

void Socket::setKeepAlive(bool on)
{

}



