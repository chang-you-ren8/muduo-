#ifndef NET_TCPCONNECTION_H
#define NET_TCPCONNECTION_H

//核心
#include "noncopyable.h"
#include "InetAddress.h"
#include "Buffer.h"
#include "Timestamp.h"
#include "Callbacks.h"

#include <memory>

class EventLoop;
class Socket;
class Channel;

class TcpConnection : noncopyable,
                      public std::enable_shared_from_this<TcpConnection>
{
public:
    TcpConnection(EventLoop *loop, int sockfd, const InetAddress &localAddr, const InetAddress &peerAddr);
    ~TcpConnection();

    void send(const string &data);
    void send(const char *data, size_t len);
    void send(Buffer *data);
    void sendInLoop(const char *data, size_t len);

    void shutdown();
    void shutdownInLoop();

    void connectEstablished();
    void connectDestroyed();

    void setNewConnectionCallback(NewConnectionCallback cb)
    { newConnectionCallback_ = std::move(cb); }

    void setMessageCallback(MessageCallback cb)
    { messageCallback_ = std::move(cb); }

    void setCloseCallback(CloseCallback cb)
    { closeCallback_ = std::move(cb); }

    EventLoop *ownerLoop() const  
    { return loop_; }

    const InetAddress &getLocalAddr() const     
    { return localAddr_; }

    const InetAddress &getPeerAddr() const    
    { return peerAddr_; }

private:
    enum ConnectionState {kConnecting, kConnected, kDisconnecting, kDisconnected};

    EventLoop *loop_;
    std::unique_ptr<Channel> channel_;
    std::unique_ptr<Socket> socket_;
    const InetAddress localAddr_;
    const InetAddress peerAddr_;

    ConnectionState state_;

    Buffer inputBuffer_;
    Buffer outputBuffer_;

    NewConnectionCallback newConnectionCallback_;
    MessageCallback messageCallback_;
    CloseCallback closeCallback_;

    void setState(ConnectionState state)
    { state_ = state; }

    ConnectionState state() const 
    { return state_; }

    void handleRead(Timestamp recieveTime);
    void handleWrite();
    void handleClose();
    void handleError();

};

#endif 