#ifndef NET_TCPSERVER_H
#define NET_TCPSERVER_H

#include "Callbacks.h"          
#include "Acceptor.h"

#include <string>
#include <set>
#include <memory>

class EventLoop;
class InetAddress;
class Channel;

class TcpServer : noncopyable
{
public:
    TcpServer(EventLoop *loop,
            const InetAddress &listenAddr, 
            const std::string &name = std::string());
    ~TcpServer(); 

    void setMessageCallback(const MessageCallback &cb)
    { messageCallback_ = cb; }
    void setConnectionCallback(const ConnectionCallback &cb)
    { connectionCallback_ = cb; }

    void start();
    bool started() const { return started; }
    const std::string &serverName()const { return name_; }
    const std::string &ipPort() const { return ipPort_; }

private:
    //loop_, acceptor_, TcpServer处于同一个线程中
    EventLoop *loop_;
    std::unique_ptr<Acceptor> acceptor_;
    std::set<TcpConnectionPtr> connections_;    //TcpConnectionPtr -- shared_ptr<TcpConnection>

    const std::string name_;
    const std::string ipPort_;
    bool started_;

    //以下两个成员是用户传递的。最后被设置成TcpConnection的成员
    MessageCallback messageCallback_;           //用户（编写服务器的人）如何处理客户端发来的数据
    ConnectionCallback connectionCallback_;     //连接创建时调用，连接断开在调用一次

    //被Acceptor::handleRead调用
    void newConnection(int sockfd, const InetAddress &peerAddr);
    //由TcpConnection::handleClose回调
    void removeConnection(const TcpConnectionPtr &conn);
    void removeConnectionInLoop(const TcpConnectionPtr &conn);
};

#endif 