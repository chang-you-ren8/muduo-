#ifndef NET_EVENTLOOP_H
#define NET_EVENTLOOP_H

#include "MutexLock.h"
#include "CurrentThread.h"
#include "Poller.h"
#include "TimerQueue.h"

#include <sys/types.h>
#include <assert.h>

#include <vector>
#include <memory>
#include <functional>

class Channel;
class EventLoop : noncopyable
{
public:
    using Functor = std::function<void()>;

    EventLoop();
    ~EventLoop();

    void loop();

    void updateChannel(Channel *channel)
    { poller_->updateChannel(channel); }

    void removeChannel(Channel *channel)
    { poller_->removeChannel(channel); }

    bool hasChannel(Channel *channel)
    { return poller_->hasChannel(channel); }

    void runInLoop(Functor cb);
    void queueInLoop(Functor cb);

    bool isInLoopThread() const
    { return tid_ == CurrentThread::tid(); }

    void assertInLoopThread() const
    { assert(isInLoopThread()); }
    
    //定时器支持
    TimerId runAt(Timestamp tim, TimerCallback cb);
    TimerId runAfter(double delaySeconds, TimerCallback cb);
    TimerId runEvery(double interval, TimerCallback cb);
    void cancel(TimerId timerId);

    pid_t tid() const
    { return tid_; }

private:
    using ChannelList = std::vector<Channel*>;

    ChannelList activeChannels_;
    std::unique_ptr<Poller> poller_;

    MutexLock mutex_;
    std::vector<Functor> pendingFunctors_;

    bool looping_;
    bool eventHandling_;

    int wakeupFd_;
    std::unique_ptr<Channel> wakeupChannel_;
    void handleRead();
    void wakeup();
    void doPendingFunctors();

    pid_t tid_;
	TimerQueue timerQueue_;

    static const int kTimeoutMs = 1000;
};

#endif 
