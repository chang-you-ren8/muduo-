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

    //std::move????
    void setReadCallback(const ReadCallback &cb)
    { readCallback_ = cb; }
    void setWriteCallback(const EventCallback &cb)
    { writeCallback_ = cb; }
    void setCloseCallback(const EventCallback &cb)
    { closeCallback_ = cb; }
    void setErrorCallback(const EventCallback &cb)
    { errorCallback_ = cb; }

    void handleEvent();
private:
    EventLoop *loop_;

    int fd_;
    int events_;
    int revents_;

    ReadCallback readCallback_;
    EventCallback writeCallback_;
    EventCallback closeCallback_;
    EventCallback errorCallback_;

    weak_ptr<void> tie_;
    bool tied_;
};

#endif 