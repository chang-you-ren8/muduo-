#ifndef NET_CHANNEL_H
#define NET_CHANNEL_H

#include "noncopyable.h"
#include "Timestamp.h"

#include <functional>
#include <memory>

class EventLoop;

class Channel : noncopyable
{
public:
    using ReadCallback = std::function<void(Timestamp)>;
    using EventCallback = std::function<void()>;

    Channel(EventLoop *loop, int fd);
    ~Channel();

    EventLoop *ownerLoop() const { return loop_; }
    int fd() const { return fd_; }
    int events() const { return events_; }
    int revents() const { return revents_; }
    void setRevents(int rev) { revents_ = rev; }
    int index() const { return index_; }
    void setIndex(int idx) { index_ = idx; }

    //internal use
    void update();
    void remove();
    void enableRead();
    void enableWrite();
    void disableRead();
    void disableWrite();
    void disableAll();
    
    //生命周期保护
    //Channel是TcpConnection的成员
    //防止在调用Channel::handleEvent的时候，TcpConnection提前销毁
    void tie(const std::shared_ptr<void> &obj);

    //std::move????
    void setReadCallback(const ReadCallback &cb)
    { readCallback_ = cb; }
    void setWriteCallback(const EventCallback &cb)
    { writeCallback_ = cb; }
    void setCloseCallback(const EventCallback &cb)
    { closeCallback_ = cb; }
    void setErrorCallback(const EventCallback &cb)
    { errorCallback_ = cb; }

    void handleEvent(Timestamp recieveTime);
private:
    EventLoop *loop_;

    int fd_;
    int events_;
    int revents_;
    int index_; //用于判断这个Channel是否已经在关注列表中(-1表示还未关注)

    ReadCallback readCallback_;
    EventCallback writeCallback_;
    EventCallback closeCallback_;
    EventCallback errorCallback_;

    weak_ptr<void> tie_;
    bool tied_;

    bool eventHandling_;

    static const int kNoneEvent;
    static const int kWriteEvent;
    static const int kReadEvent;
};

#endif 