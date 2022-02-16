#include "TcpServer.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "Buffer.h"
#include "Timestamp.h"

#include <stdio.h>

#include <string>
using std::string;

void onMessage(const TcpConnectionPtr &conn, Buffer *data, Timestamp recieveTime)
{
    string msg(buf->retrieveAllAsString());
    printf("%s", msg.c_str());
    conn->send(msg);
}

void onConnection(const TcpConnectionPtr &conn)
{
    printf("%s:%d------>%s:%d\n",
            conn->localAddr().ipToString().c_str(), 
            conn->localAddr().hostEndianPort(),
            conn->peerAddr().ipToString().c_str(),
            conn->peerAddr().hostEndianPort()
            );
    conn->send("hello\n");
}

int main()
{
    InetAddress listenAddr(12345);
    EventLoop loop;
    TcpServer server(loop, listenAddr);   
    server.setMessageCallback()
    server.start();

    return 0;
}



