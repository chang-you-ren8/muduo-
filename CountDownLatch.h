#ifndef BASE_COUNTDOWNLATCH_H
#define BASE_COUNTDOWNLATCH_H

//倒计时类，这个东西有什么用呢？
//1.主线程创建一个子线程，等待子线程完成一些操作之后，主线程再继续执行
//使用MutexLock和Condition来实现

#include "MutexLockGuard.h"
#include "MutexLock.h"
#include "Condition.h"

class CountDownLatch : noncopyable
{
public:
    explicit CountDownLatch(int count);

    ~CountDownLatch();

    void countDown();
    void wait();

private:
    int count_;
    MutexLock mutex_;
    Condition cond_;
};

#endif