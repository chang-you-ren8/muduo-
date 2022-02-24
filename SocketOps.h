#ifndef NET_SOCKETOPS_H
#define NET_SOCKETOPS_H

#include <sys/socket.h>

class InetAddress;

namespace sockets
{
    void bindOrDie(int sockfd, const InetAddress &listenAddr);
    void listenOrDie(int listenfd);
    int accept(int listenfd, InetAddress *peerAddr);
    int createNonBlockOrDie();
    void close(int sockfd);

    InetAddress getLocalAddr(int sockfd);
    InetAddress getPeerAddr(int sockfd);

    ssize_t write(int fd, const void *buf, size_t count);
    ssize_t read(int fd, void *buf, size_t count);
}

#endif