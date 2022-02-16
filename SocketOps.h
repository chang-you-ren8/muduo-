#ifndef NET_SOCKETOPS_H
#define NET_SOCKETOPS_H

//先简单实现，毕竟这个不是核心。。。
//只考虑ipv4
#include <sys/types.h>
#include <sys/socket.h>

namespace sockets
{
int createNonblockFdOrDie(sa_family_t family);
int connect(int sockfd, const struct sockaddr *addr, socklen_t len);
void bindOrDie(int sockfd, const struct sockaddr *addr, socklen_t len);
void listenOrDie(int sockfd);
int accept(int sockfd, struct sockaddr *addr, socklen_t *len);
ssize_t read(int sockfd, void *buf, size_t count);
ssize_t write(int sockfd, const void *buf, size_t count);
void close(int sockfd);
void shutdownWrite(int sockfd);
ssize_t readv(int fd, struct iovec *iov, int iovcnt);
}

#endif 