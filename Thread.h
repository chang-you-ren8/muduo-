#ifndef BASE_THREAD_H
#define BASE_THREAD_H
//基于对象的设计模式。
//其实就是执行一个函数

#include "CountDownLatch.h"

#include <sys/types.h> //for pid_t

#include <functional>

namespace detail
{
class ThreadData : noncopyable
{
public:
    using ThreadFunc = std::function<void()>;

    ThreadData(ThreadFunc func, pid_t *tid, CountDownLatch *latch)
        :   func_(std::move(func)),
            tid_(tid),
            latch_(latch)
    { }

    ~ThreadData() = default;

    ThreadFunc func_;
    pid_t *tid_;
    CountDownLatch *latch_;    
};

} //detail

class Thread : noncopyable
{
public:
    using ThreadFunc = std::function<void()>;

    //c++11移动语义
    explicit Thread(ThreadFunc func);
    ~Thread();

    void start();
    void join();
    
    pid_t tid() const { return tid_; }

    pthread_t pthreadId() const { return pthreadId_; }
private:
    ThreadFunc func_;
    bool started_;
    bool joined_;
    pthread_t pthreadId_;
    pid_t tid_;             //次线程对应的进程的pid, 注意(linux中每个线程其实就是一个LWP)
    CountDownLatch latch_;  //for tid_

    static void *startInThread(void *arg);
};

#endif