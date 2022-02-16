#include "InetAddress.h"

#include <netinet/in.h> //for INET_ADDRSTRLEN(ipv4点分十进制最大长度包括末尾的null)

#include <string.h>
#include <assert.h>

InetAddress::InetAddress(uint16_t port)
{
    ::bzero(&sockaddr_, sizeof(sockaddr_));
    sockaddr_.sin_family = AF_INET;
    sockaddr_.sin_addr.s_addr = ::htonl(INADDR_ANY);
    sockaddr_.sin_port = ::htons(port);
}

InetAddress::InetAddress(const std::string &ip, uint16_t port)
{
    ::bzero(&sockaddr_, sizeof(sockaddr_));
    sockaddr_.sin_family = AF_INET;
    int ret = ::inet_pton(AF_INET, ip.c_str(), &sockaddr_.sin_addr);
    assert(ret == 1);
    sockaddr_.sin_port = ::htons(port);
}

std::string InetAddress::ipToString() const 
{
    char str[INET_ADDRSTRLEN];
    ::inet_ntop(AF_INET, &sockaddr_.sin_addr, str, sizeof str);
    return str;
}

