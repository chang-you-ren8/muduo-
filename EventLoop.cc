#include "EventLoop.h"
#include "Poller.h"
#include "Channel.h"
#include "MutexLockGuard.h"

#include <assert.h>
#include <sys/eventfd.h>

#include <functional>

int createNonblockingOrDie()
{
    int fd = ::eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK);
    assert(fd >= 0);
    return fd;
}

const int EventLoop::kTimeoutMs;

EventLoop::EventLoop()
	:	poller_(Poller::newDefaultPoller(this)),
        mutex_(),
        looping_(false),
        eventHandling_(false),
        wakeupFd_(createNonblockingOrDie()),
        wakeupChannel_(new Channel(this, wakeupFd_)),
        tid_(CurrentThread::tid()),
		timerQueue_(this)
{
    wakeupChannel_->setReadCallback(std::bind(&EventLoop::handleRead, this));
    wakeupChannel_->enableRead();
}

EventLoop::~EventLoop()
{
    ::close(wakeupFd_);
}

//想想自己曾经写的
void EventLoop::loop()
{
    assert(!looping_);
    looping_ = true;
    
    while(looping_)
    {
        activeChannels_.clear();
        Timestamp pollReturnTime = poller_->poll(&activeChannels_, kTimeoutMs);
    
        //for(const auto &ch : activeChannels_) ???
        //我对这个唤醒的把握还是有问题。问题出在, doPendingFunctors, handlevent可能产生的所有情况
        //wakeup的目的是为了让pendingFunctors尽快得到执行,那么如何才能做到这点呢？
        eventHandling_ = true;
        for(auto &ch : activeChannels_)
            ch->handleEvent(pollReturnTime);
        eventHandling_ = false;

        doPendingFunctors();
    }
}

void EventLoop::runInLoop(Functor cb)
{
    if(isInLoopThread())
        cb();
    else   
        queueInLoop(cb);
}

void EventLoop::queueInLoop(Functor cb)
{
    {
        MutexLockGuard lock(mutex_);
        pendingFunctors_.push_back(std::move(cb));
    }

    //什么情况？
    if(!isInLoopThread() || eventHandling_)
        wakeup();
}

TimerId EventLoop::runAt(Timestamp tim, TimerCallback cb)
{
    return timerQueue_.addTimer(std::move(cb), tim, 0.0);
}

TimerId EventLoop::runAfter(double delaySeconds, TimerCallback cb)
{
    Timestamp when = Timestamp::addTime(Timestamp::now(), delaySeconds);
    return timerQueue_.addTimer(std::move(cb), when, 0.0);
}

TimerId EventLoop::runEvery(double interval, TimerCallback cb)
{
    Timestamp when = Timestamp::addTime(Timestamp::now(), interval);
    return timerQueue_.addTimer(std::move(cb), when, interval);
}

void EventLoop::cancel(TimerId timerId)
{
    timerQueue_.cancel(timerId);
}

void EventLoop::handleRead()
{
    uint64_t val;
    int n = ::read(wakeupFd_, &val, sizeof(val));
    assert(n == static_cast<int>(sizeof(val)));
}

void EventLoop::wakeup()
{
    uint64_t val = 1;
    int n = ::write(wakeupFd_, &val, sizeof(val));
    assert(n == static_cast<int>(sizeof(val)));
}

void EventLoop::doPendingFunctors()
{
    std::vector<Functor> functors;
    //必须这样做（否则如果pendingFunctors中的某个Functor调用queueInLoop就会造成死锁）
    {
        MutexLockGuard lock(mutex_);
        functors.swap(pendingFunctors_);
    }

    for(const auto &func : functors)
        func();
}




