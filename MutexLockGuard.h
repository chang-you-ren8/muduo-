#ifndef BASE_MUTEXLOCKGUARD_H
#define BASE_MUTEXLOCKGUARD_H

#include "noncopyable.h"
#include "MutexLock.h"

//RAII实现自动解锁
class MutexLockGuard : noncopyable
{
public:

    //构造的时候加锁
    MutexLockGuard(MutexLock &mutex)
        :   mutex_(mutex)
    {
        mutex_.lock();
    }

    //析够函数自动解锁
    ~MutexLockGuard()
    {
        mutex_.unlock();
    }

private:
    MutexLock& mutex_;
};

#endif