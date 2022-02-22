#ifndef NET_CHANNEL_H
#define NET_CHANNEL_H

#include "EventLoop.h"
#include "Timestamp.h"

#include <poll.h>

#include <functional>
#include <memory>


class Channel : noncopyable
{
public:
    using ReadCallback = std::function<void(Timestamp)>;
    using EventCallback = std::function<void()>;
    
    Channel(EventLoop *loop, int fd);
    ~Channel();

    void enableRead()
    {
        events_ |= kReadEvent;
        update();
    }

    void disableRead()
    {
        events_ &= ~kReadEvent;
        update();
    }

    void enableWrite()
    {
        events_ |= kWriteEvent;
        update();
    }

    void disableWrite()
    {
        events_ &= ~kWriteEvent;
        update();
    }

    void disableAll()
    {
        events_ = kNoneEvent;
        update();
    }

    void setRevents(int revents)
    { revents_ = revents; }

    int events() const
    { return events_; }

    void setIndex(int idx)
    { index_ = idx; }

    int index() const
    { return index_; }

    int fd() const
    { return fd_; }    

    void remove()
    { loop_->removeChannel(this); }

    void setReadCallback(ReadCallback cb)
    { readCallback_ = std::move(cb); }

    void setWriteCallback(EventCallback cb)
    { writeCallback_ = std::move(cb); }

    void setCloseCallback(EventCallback cb)
    { closeCallback_ = std::move(cb); }

    void setErrorCallack(EventCallback cb)
    { errorCallback_ = std::move(cb); }

    void handleEvent(Timestamp recieveTime);

    void tie(const std::shared_ptr<void> &obj);

private:
    EventLoop *loop_;
    int fd_;
    int index_;

    bool eventHandling_;
    bool tied_;
    std::weak_ptr<void> tie_;

    int events_;
    int revents_;

    ReadCallback readCallback_;
    EventCallback writeCallback_;
    EventCallback closeCallback_;
    EventCallback errorCallback_;

    static const int kReadEvent = POLLIN;
    static const int kWriteEvent = POLLOUT;
    static const int kNoneEvent = 0;

    void update()
    { loop_->updateChannel(this); }
};

#endif 