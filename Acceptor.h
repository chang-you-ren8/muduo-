#ifndef NET_ACCEPTOR_H
#define NET_ACCEPTOR_H

#include "Socket.h"
#include "Channel.h"
#include "Callbacks.h"

#include <sys/types.h>
#include <unistd.h>     

class EventLoop;
class InetAddress;

class Acceptor : noncopyable
{
public:
    Acceptor(EventLoop *loop, const InetAddress &listenAddr);
    ~Acceptor();

    void listen();
    bool listening() { return listening_; }
    
    void setNewConnectionCallback(const NewConnectoinCall &cb)
    { newConnectionCallback_ = cb; }

private:
    void handleRead();

    EventLoop *loop_;
    Socket acceptSocket_;
    Channel acceptChannel_;
    int idleFd_;
    bool listening_;
    NewConnectoinCall newConnectionCallback_;
}
#endif 