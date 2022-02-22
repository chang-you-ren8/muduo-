#ifndef NET_SOCKET_H
#define NET_SOCKET_H

#include "noncopyable.h"

class InetAddress;

//用于管理套接字的生命周期，设置关于套接字的选项
class Socket : noncopyable
{
public:
    explicit Socket(int sockfd);
    ~Socket(); 
    
    int fd() const { return sockfd_; }
    void bind(const InetAddress &listenAddr);
    void listen();
    int accept(InetAddress *peerAddr);

    void setReuseAddr(bool on);
    void setTcpNoDelay(bool on);
    void setKeepAlive(bool on);
    
private:
    const int sockfd_;
};

#endif 