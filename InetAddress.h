#ifndef NET_INETADDRESS_H
#define NET_INETADDRESS_H

//值语义
//对ip地址和port的封装
//暂时只支持ipv4

#include <sys/socket.h> //struct sockaddr
#include <arpa/inet.h>  //struct sockaddr_in
#include <sys/types.h>
#include <stdint.h>

#include <string>

//ipv4地址封装
class InetAddress
{
public:
    //服务器端调用
    explicit InetAddress(uint16_t port);
    //客户端调用(点分十进制ip地址, 端口号)
    InetAddress(const std::string &ip, uint16_t port);

    void setSockAddr(struct sockaddr_in addr)
    { sockaddr_ = addr; }

    struct sockaddr_in getSockAddr() const
    { return sockaddr_; }
    socklen_t getAddrSize() const
    { return sizeof(sockaddr_); }

    uint16_t hostEndianPort() const 
    { return ::ntohs(netEndianPort()); }

    uint16_t netEndianPort() const
    { return sockaddr_.sin_port; }

    //点分十进制形式ip地址
    std::string ipToString() const;

    sa_family_t family() const 
    { return sockaddr_.sin_family; }

private:
    struct sockaddr_in sockaddr_;
};

#endif 