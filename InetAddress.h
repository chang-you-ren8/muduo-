#ifndef NET_INETADDRESS_H
#define NET_INETADDRESS_H

#include "copyable.h"

#include <sys/socket.h>
#include <arpa/inet.h>

#include <string>

class InetAddress : copyable
{
public:
    InetAddress();
    //服务器端使用
    explicit InetAddress(uint16_t port);
    //客户端使用
    InetAddress(const std::string &ip, uint16_t port);

    sa_family_t family() const 
    { return sockaddr_.sin_family; }

    struct sockaddr_in getSockaddr() const 
    { return sockaddr_; }

    void setSockaddr(struct sockaddr_in sockaddr)
    { sockaddr_ = sockaddr; }

    std::string ipToString() const;
    std::string portToString() const;

    //主机字节序
    int ip() const
    { return ::ntohl(sockaddr_.sin_addr.s_addr); }

    int port() const
    { return ::ntohs(sockaddr_.sin_port); }

private:
    struct sockaddr_in sockaddr_;
};

#endif 