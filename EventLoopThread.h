#ifndef NET_EVENTLOOPTHREAD_H
#define NET_EVENTLOOPTHREAD_H

//基于对象。EventLoopThread内部包含一个Thread对象

#include "Thread.h"
#include "CountDownLatch.h"

class EventLoop;

class EventLoopThread : noncopyable
{
public:
    EventLoopThread();
    ~EventLoopThread();

    //会不会在loop_未完成初始化，就调用了getLoop????
    //应该需要CountDownLatch ??? 
    //muduo的实现使用了MutexLock 和 Condition来实现的。
    //那么使用CountDownLatch到底是否可以呢？
    //首先这个EventLoopThread应用场景是什么？
    //我认为使用CountDownLatch可以做到线程安全，startLoop只会被调用一次
    EventLoop *startLoop();

private:
    Thread thread_;
    CountDownLatch latch_;
    EventLoop *loop_;
    bool looping_; 

    void threadFunc();
};

#endif 