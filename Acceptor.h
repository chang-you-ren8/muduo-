#ifndef NET_ACCEPTOR_H
#define NET_ACCEPTOR_H

//如果不懂不知道自己的需求？那么我如何设计自己的软件系统。。。。
//Acceptor就是对listenfd的封装，那么平常使用listenfd的时候我是怎么用的？

#include "noncopyable.h"

#include <memory>

class InetAddress;
class Socket;
class EventLoop;
class Channel;

class Acceptor : noncopyable
{
public:
    using NewConnectionCallback = std::function<void(int, const InetAddress &)>;

    Acceptor(EventLoop *loop, const InetAddress &listenAddr);
    ~Acceptor();

    void setNewConnectionCallback(NewConnectionCallback cb)
    { newConnectionCallback_ = std::move(cb); }

    void listen();

private:
    std::unique_ptr<Socket> acceptSocket_;
    std::unique_ptr<Channel> acceptChannel_;
    NewConnectionCallback newConnectionCallback_;
    int idleFd_;

    //accept一个新的连接
    void handleRead(); 
};

#endif 
