#ifndef NET_TCPCONNECTION_H
#define NET_TCPCONNECTION_H

#include "Timestamp.h"
#include "noncopyable.h"
#include "Callbacks.h"
#include "Buffer.h"
#include "InetAddress.h"

#include <memroy>

class EventLoop;
class Channel;
class Socket;
//当一个对象被shared_ptr管理，又有其他函数回调这个对象的成员函数（异步回调），这个时候就需要继承自
//enable_shared_form_this, 通过shared_from_this()函数来返回一个管理this指针的shared_ptr。可以放置对象被过早释放掉
//为什么不使用shared_ptr<TcpConnection>(this)呢？这会造成两个无关的shared_ptr都管理this,会导致多次释放
//深入的细节还未搞懂
class TcpConnection : noncopyable,
                      public std::enable_shared_form_this<TcpConnection>
{
public:
    TcpConnection(EventLoop *loop,
                int sockfd,
                const InetAddress &localAddr,
                const InetAddress &peerAddr);
    ~TcpConnection();

    //实际上所有的send函数最终都调用sendInLoop
    //我们的模型是OneThread + OneEventLoop
    //只要这个TcpConnection由loop_管理，那么
    //所有对这个TcpConnection的操作，最终都放到EventLoop所在的线程处理
    void send(const char *data, size_t len);
    void send(const std::string &data);
    void send(Buffer *data);
    void sendInLoop(const char *data, size_t len);

    bool connected() const { return ConnectionState == kConnected; }
    bool disConnected() const { return ConnectionState == kDisconnected; }
    void setState(ConnectionState state) { ConnectionState = state; }

    //服务器端主动关闭连接
    //对方关闭连接标志：read返回0
    void shutdown();
    void shutdownInLoop(); 

    void connectEstablished();  //连接刚建立的时候调用,由TcpServer::newConnection将这个函数runInLoop到EventLoop中
    void connectDestroyed();    //连接销毁的时候调用,由TcpServer::removeConnection()将这个函数runInLoop到EventLoop中

    void setMessageCallback(const MessageCallback &cb)
    { messageCallback_ = cb; }
    void setConnectionCallback(const ConnectionCallback &cb)
    { connectionCallback_ = cb }
    void setCloseCallback(const CloseCallback &cb)
    { closeCallback_ = cb; }

    const InetAddress &localAddr() const 
    { return localAddr_; }
    const InetAddress &peerAddr() const 
    { return peerAddr_; }

    Buffer *inputBuffer()
    { return inputBuffer_; }
    Buffer *outputBuffer()
    { return outputBuffer_; }

private:
    enum ConnectionState {kDisconnecting, kDisconnected, kConnected, kConnecting};
    EventLoop *loop_;
    std::unique_ptr<Channel> channel_;
    std::unique_ptr<Socket> socket_;
    ConnectionState state_;

    MessageCallback messageCallback_;
    ConnectoinCallback connectionCallback_;
    CloseCallback closeCallback_;
    
    Buffer inputBuffer_;
    Buffer outputBuffer_;
    const InetAddress localAddr_;
    const InetAddress peerAddr_;

    //内部使用，由EventLoop通过Channel::handleEvent自动调用
    void handleRead(Timestamp receiveTime);
    void handleWrite();
    void handleClose();
    void handleError();
};

#endif 