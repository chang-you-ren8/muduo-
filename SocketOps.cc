#include "SocketOps.h"

#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/uio.h> //for readv

int sockets::createNonblockFdOrDie(sa_family_t family)
{
    int fd = ::socket(family, SOCK_STREAM, 0);
    assert(fd >= 0);
    int old_flag = ::fcntl(fd, F_GETFL);
    int new_flag = old_flag | O_NONBLOCK;
    int ret = ::fcntl(fd, F_SETFL, new_flag);
    assert(ret == 0);
    return fd;
}

int sockets::connect(int sockfd, const struct sockaddr *addr, socklen_t len)
{
    return ::connect(sockfd, addr, len);
}

void sockets::bindOrDie(int sockfd, const struct sockaddr *addr, socklen_t len)
{
    int ret = ::bind(sockfd, addr, len);
    assert(ret == 0);
}

void sockets::listenOrDie(int sockfd)
{
    int ret = ::listen(sockfd, SOMAXCONN);
    assert(ret == 0);
}

int sockets::accept(int sockfd, struct sockaddr *addr, socklen_t *len)
{
    return ::accept(sockfd, addr, len);
}

ssize_t sockets::read(int sockfd, void *buf, size_t count)
{
    return ::read(sockfd, buf, count);
}

ssize_t sockets::write(int sockfd, const void *buf, size_t count)
{
    return ::write(sockfd, buf, count);
}

void sockets::close(int sockfd)
{
    int ret = ::close(sockfd);
    assert(ret == 0);
}

void sockets::shutdownWrite(int sockfd)
{
    int ret = ::shutdown(sockfd, SHUT_WR);
    assert(ret == 0);
}

ssize_t sockets::readv(int fd, struct iovec *iov, int iovcnt)
{
    return ::readv(fd, iov, iovcnt);
}