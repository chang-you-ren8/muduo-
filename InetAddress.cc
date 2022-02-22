#include "InetAddress.h"

#include <string.h>

InetAddress::InetAddress() = default;

InetAddress::InetAddress(uint16_t port)
{
    sockaddr_.sin_family = AF_INET;

    sockaddr_.sin_addr.s_addr = ::htonl(INADDR_ANY);
    sockaddr_.sin_port = ::htons(port);
}

//我日。。。。下面错了，上面居然。。。。。
InetAddress::InetAddress(const std::string &ip, uint16_t port)
{   
    sockaddr_.sin_family = AF_INET;
    inet_pton(AF_INET, ip.c_str(), &sockaddr_.sin_addr);
    sockaddr_.sin_port = ::htons(port); 
}

std::string InetAddress::ipToString() const
{
    char buf[INET_ADDRSTRLEN];
    ::inet_ntop(AF_INET, &sockaddr_.sin_addr, buf, sizeof(buf));
    return std::string(buf);
}

std::string InetAddress::portToString() const    
{
    char buf[32];
    ::snprintf(buf, sizeof(buf), "%d", port());
    return std::string(buf);
}
