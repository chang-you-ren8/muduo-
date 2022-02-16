#ifndef NET_EVENTLOOP_H
#define NET_EVENTLOOP_H

//EventLoop可以说是整个muduo库最核心的内容了
//那么这个EventLoop到底应该设计成什么样的呢？
//他应该提供什么样的接口给客户呢？

//暂时不考虑定时操作

//1.对外只提供loop, quit, runInLoop, queueInLoop, assertInLoopThread
//2.对内通过wakeupFd_, wakeupChannel_, handleRead实现唤醒(就是poll可能因为没有fd是激活的而阻塞)

#include "MutexLock.h"

#include <sys/types.h>

#include <vector>
#include <functional>

class Channel;
class Poller;

class EventLoop : noncopyable
{
public:
    using Functor = std::function<void()>;

    EventLoop();
    ~EventLoop();

    void loop();
    void quit();

    void assertInLoopThread();
    bool isInLoopThread();      //使用当前线程对应的pid
    //thread safe
    void runInLoop(const Functor &cb);
    void queueInLoop(const Functor &cb);

    //转交给poller_来处理
    //以下三个线程只可能在EventLoop所处的线程被调用
    //internal use
    bool hasChannel(Channel *channel);
    void updateChannel(Channel *channel);
    void removeChannel(Channel *channel);
private:
    using ChannelList = std::vector<Channel*>;
    using FunctorQueue = std::vector<Functor>;

    //用于唤醒loop
    int wakeupFd_;
    std::unique_ptr<Channel> wakeupChannel_;
    void wakeup();
    void handleRead();

    //处理pendingFunctor
    bool callingFunctor_;
    MutexLock mutex_;
    FunctorQueue pendingFunctors_;
    void doPendingFunctors();

    bool looping_;
    bool quit_;
    std::unique_ptr<Poller> poller_;
    ChannelList activeChannels_; 

    pid_t tid_;

    const static int kPollTimeoutMS = 1000;
};

#endif