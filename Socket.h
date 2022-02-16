#ifndef NET_SOCKET_H
#define NET_SOCKET_H

#include "noncopyable.h"

class InetAddress;

//Socket对sockfd的生命周期进行管理，但是Socket不负责创建sockfd
class Socket : noncopyable
{
public:
    explicit Socket(int sockfd)
        :sockfd_(sockfd)
    { }

    ~Socket();

    int fd() const { return sockfd_; }
    void bind(const InetAddress &localAddr);
    void listen();
    int accept(InetAddress *peerAddr);
    void shutdownWrite();

    void setTcpNoDelay(bool on);    //我猜测和Tcp的慢启动有关
    void setReuseAddr(bool on);
    void setReusePort(bool on);     //Linux内核新特性
    void setKeepAlive(bool on);

private:
    const int sockfd_;
};

#endif 