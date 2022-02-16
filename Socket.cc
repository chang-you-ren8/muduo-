#include "Socket.h"
#include "InetAddress.h"
#include "SocketOps.h"

#include <sys/socket.h>
#include <sys/types.h>

#include <string.h>
#include <stdio.h>
#include <assert.h>


Socket::~Socket()
{
    sockets::close(sockfd_);
}

void Socket::bind(const InetAddress &addr)
{
    socklen_t len = addr.getAddrSize();
    sockets::bindOrDie(sockfd_, (struct sockaddr *)&addr.getSockAddr(), len);
}

void Socket::listen()
{
    sockets::listenOrDie(sockfd_);
}

int Socket::accept(InetAddress *peerAddr)
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    int connfd = sockets::accept(sockfd_, (struct sockaddr *)&addr, &len);
    if(connfd >= 0)
    {
        peerAddr->setSockAddr(addr);
    }
    return connfd;
}

void Socket::shutdownWrite()
{
    sockets::shutdownWrite(sockfd_);
}

void Socket::setTcpNoDelay(bool on)
{
    int optval = on ? 1 : 0;
    int ret = ::setsockopt(sockfd_, IPPROTO_TCP, TCP_NODELYA,
                &optval, static_cast<socklen_t>(sizeof(optval)));
    assert(ret == 0);
}

void Socket::setReuseAddr(bool on)
{
    int optval = on ? 1 : 0;
    int ret = ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR,
                &optval, static_cast<socklen_t>(sizeof(optval)));
    assert(ret == 0);
}

//SO_KEEPALIVE的工作工程
void Socket::setKeepAlive(bool on)
{
    int optval = on ? 1 : 0;
    int ret = ::setsockopt(sockfd_, SOL_SOCKET, SO_KEEPALIVE,
                &optval, static_cast<socklen_t>(optval)); 
    assert(ret == 0);
}