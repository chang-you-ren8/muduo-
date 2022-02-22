#include "TcpServer.h"
#include "InetAddress.h"

#include <stdio.h>
#include <unistd.h>

//不处理连接的关闭，这个时候还未封装TcpConnection...
void newConnection(int sockfd, const InetAddress &peerAddr)
{
    printf("newConnection peerIp=%s, peerPort=%s\n",
        peerAddr.ipToString().c_str(),
        peerAddr.portToString().c_str());
}

void messageCallback()