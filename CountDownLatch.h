#ifndef BASE_COUNTDOWNLATCH_H
#define BASE_COUNTDOWNLATCH_H

//实际上相当于计数器，当计数到0时，才能继续执行。
//相当屏障
#include "noncopyable.h"
#include "Condition.h"
#include "MutexLock.h"
#include "MutexLockGuard.h"

class CountDownLatch : noncopyable
{
public:
    explicit CountDownLatch(int count);
    ~CountDownLatch();

    void wait();
    void countDown();
    int getCount() const;

private:
    mutable MutexLock mutex_;   //在const 成员函数中也可以修改这个成员
    Condition cond_;
    int count_;                 //计数到0，就调用cond_.notifyAll()
};

#endif 