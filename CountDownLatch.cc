#include "CountDownLatch.h"

CountDownLatch::CountDownLatch(int count)
    :   mutex_(),
        cond_(mutex_),
        count_(count)
{

}


CountDownLatch::~CountDownLatch()
{

}

void CountDownLatch::wait()
{
    MutexLockGuard lock(mutex_);
    while(count_ > 0)
        cond_.wait();
}

void CountDownLatch::countDown()
{
    MutexLockGuard lock(mutex_);
    if(--count_ == 0)
        cond_.notifyAll();
}

int CountDownLatch::getCount() const 
{
    //mutex_之所以为mutable的原因
    MutexLockGuard lock(mutex_);
    return count_;
}
