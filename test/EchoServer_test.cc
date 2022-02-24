#include "TcpServer.h"
#include "EventLoop.h"
#include "InetAddress.h"

#include <stdio.h>
#include <unistd.h>

#include <functional>
#include <memory>

class EchoServer
{
public:
    EchoServer(EventLoop *loop, const InetAddress &listenAddr, int threadNum)
        :   server_(loop, listenAddr),
            listenAddr_(listenAddr)
    {
        server_.setThreadNum(threadNum);
        server_.setNewConnectionCallback(std::bind(&EchoServer::newConnection, this, _1));
        server_.setMessageCallback(std::bind(&EchoServer::onMessage, this, _1, _2, _3));
    }

    void start()
    {
        server_.start();
    }

private:
    TcpServer server_;
    const InetAddress listenAddr_;

    void newConnection(const TcpConnectionPtr &conn)
    {
        printf("create newConnection: peerIp = %s, peerPort = %s, in thread:%d\n",
            conn->getPeerAddr().ipToString().c_str(),
            conn->getPeerAddr().portToString().c_str(),
            CurrentThread::tid());

        conn->send("Hello World\n");
    }

    void onMessage(const TcpConnectionPtr &conn, Buffer *buffer, Timestamp recieveTime)
    {
        conn->send(buffer->retrieveAllAsString());
    }
};


int main()
{
    InetAddress listenAddr(12345);
    EventLoop loop;
    EchoServer server(&loop, listenAddr, 4);
    server.start();
    loop.loop();

    return 0;
}
