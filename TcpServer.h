#ifndef NET_TCPSERVER_H
#define NET_TCPSERVER_H

//TcpServer应该如何设计？？？？？
//用户只需要包含TcpSever.h即可，用户不需要自己包含TcpConnection.h，否则我的头文件设计就有问题。
//也许我现在应该停下来看看需求再来实现，毕竟如果我不知道自己的需求我你妈实现个屁 ？？？？？/
//我还没有掌握软件设计的基本方法，甚至一无所知。。。。

#include "TcpConnection.h"
#include "EventLoopThreadPool.h"
#include "Callbacks.h"
#include "InetAddress.h"

#include <memory>
#include <set>

class EventLoop;
class Acceptor;

class TcpServer : noncopyable
{
public:
    TcpServer(EventLoop *baseLoop, const InetAddress &listenAddr, const string name = string());
    ~TcpServer();

    void setNewConnectionCallback(NewConnectionCallback cb)
    { newConnectionCallback_ = std::move(cb); }

    void setMessageCallback(MessageCallback cb)
    { messageCallback_ = std::move(cb); }

    void start();

    void setThreadNum(int threadNum)
    { pool_.setThreadNum(threadNum); }

    const string& name() const
    { return name_; }

private:
    EventLoop *baseLoop_;
    const InetAddress listenAddr_;
    const string name_;
    EventLoopThreadPool pool_;
    std::unique_ptr<Acceptor> acceptor_;
    bool started_;

    std::set<TcpConnectionPtr> connections_;

    NewConnectionCallback newConnectionCallback_;
    MessageCallback messageCallback_;

    void newConnection(int sockfd, const InetAddress &peerAddr);
    void removeConnection(const TcpConnectionPtr &conn);
    void removeConnectionInLoop(const TcpConnectionPtr &conn);
};

#endif 