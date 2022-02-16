#include "EventLoop.h"
#include "Poller.h"
#include "Channel.h"
#include "CurrentThread.h"
#include "Timestamp.h"
#include "MutexLockGuard.h"

#include <stdint.h>
#include <assert.h>
#include <sys/eventfd.h>
#include <unistd.h>

int createEventfd()
{
    int efd = ::eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK);
    assert(efd >= 0);
    return efd;
}

EventLoop::EventLoop()
    :   wakeupFd_(createEventfd()),
        wakeupChannel_(new wakeupChannel_(wakeupFd_, this)),
        callingFunctor_(false),
        mutex_(),
        looping_(false),
        quit_(false),
        poller_(Poller::defautPoller(this)),
        tid_(CurrentThread::tid())
{
    wakeupChannel_->setReadCallback(std::bind(&EventLoop::handleRead));
    wakeupChannel_->enableRead();
}

//我对这个析构函数被调用时如何处理还不清楚
EventLoop::~EventLoop()
{
    wakeupChannel_->disableAll();
    wakeupChannel_->removeChannel();
}

void EventLoop::loop()
{
    assert(!looping_);
    looping_ = true;

    while(!quit_)
    {
        activeChannels_.clear();
        Timestamp pollReturnTime = poller_->poll(&*activeChannels_.begin(), kPollTimeoutMS);

        for(auto it = activeChannels_.begin(); it != activeChannels_.begin(); ++it)
        {
            Channel *ch = *it;
            ch->handleRead();
        }

        doPendingFunctors();
    }
}

void EventLoop::assertInLoopThread()
{
    assert(isInLoopThread());
}

bool EventLoop::isInLoopThread()
{
    return tid_ == CurrentThread::tid();
}

void EventLoop::runInLoop(const Functor &cb)
{
    if(isInLoopThread())
        cb();
    else
        queueInLoop(cb);
}

void EventLoop::queueInLoop(const Functor &cb)
{
    {
        MutexLockGuard lock(mutex_);
        pendingFunctors_.push_back(cb);
    } 

    if(!isInLoopThread() || callingFunctor_)
        wakeup();
}

void EventLoop::wakeup()
{
    uint64_t val = 1;
    auto n = ::write(wakeupFd_, &val, sizeof val);
    assert(n == sizeof val);        //简单测试，实际上应该使用Log,而不是直接终止进程
}

void EventLoop::handleRead()
{
    uint64_t val;
    auto n = ::read(wakeupFd_, &val, sizeof val);
    assert(n == sizeof val);
}

void EventLoop::doPendingFunctors()
{
    std::vector<Functor> functors;
    {
        MutexLockGuard lock(mutex_);
        functors.swap(pendingFunctors_);
    }

    callingFunctor_ = true;
    for(const auto & func : functors)
        func();
    callingFunctor_ = false;
}

bool EventLoop::hasChannel(Channel *channel)
{
    assert(channel->ownerLoop() == this);
    assertInLoopThread();
    return poller_->hasChannel(channel);
}

void EventLoop::updateChanne(Channle *channel)
{
    assert(channel->ownerLoop() == this);
    assertInLoopThread();
    poller_->updateChanne(channel);
}

void EventLoop::removeChannel(Channel *channel)
{
    assert(channel->ownerLoop() == this);
    assertInLoopThread();
    poller_->removeChannel(channel);
}