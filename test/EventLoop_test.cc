//测试驱动。。。。。。。。。
#include "EventLoop.h"
#include "Acceptor.h"
#include "InetAddress.h"

#include <stdio.h>

#include <functional>
#include <string>

using std::placeholders::_1;
using std::placeholders::_2;

void newConnection(int fd, const InetAddress &peerAddr)
{
    printf("newConnection peerAddr: ip=%s, port=%s\n",
        peerAddr.ipToString().c_str(),
        peerAddr.portToString().c_str());
    
}

int main()
{
    //这里只是简单的测试，实际上Accetor类是给TcpServer类使用的。
    InetAddress listenAddr(12345);
    EventLoop loop;
    Acceptor acceptor(&loop, listenAddr);
    acceptor.setNewConnectionCallback(std::bind(&newConnection, _1, _2));
    acceptor.listen();
    loop.loop();

    return 0;
}