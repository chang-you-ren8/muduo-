#include "CountDownLatch.h"


CountDownLatch::CountDownLatch(int count)
    :   count_(count),
        mutex_(),
        cond_(mutex_)
{
    assert(count >= 0);
}

CountDownLatch::~CountDownLatch() = default;

void CountDownLatch::countDown()
{
    MutexLockGuard lock(mutex_);
    if(--count_ == 0)
        cond_.notifyAll();
}

void CountDownLatch::wait()
{
    MutexLockGuard lock(mutex_);
    while(count_ > 0)
        cond_.wait();
}

