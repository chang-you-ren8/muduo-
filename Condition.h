#ifndef BASE_CONDITION_H
#define BASE_CONDITION_H

#include "MutexLock.h"
#include "MutexLockGuard.h"

class Condition : noncopyable
{
public:
    explicit Condition(MutexLock &mutex)
        :mutex_(mutex)
    {
        int ret = pthread_cond_init(&cond_, NULL);
        assert(ret == 0);
    }

    ~Condition()
    {
        int ret = pthread_cond_destroy(&cond_);
        assert(ret == 0);
    }

    void wait()
    {
        int ret = pthread_cond_wait(&cond_, mutex_.getPthreadMutex());
        assert(ret == 0);
    }

    void notify()
    {
        int ret = pthread_cond_signal(&cond_);
        assert(ret == 0);
    }

    void notifyAll()
    {
        int ret = pthread_cond_broadcast(&cond_);
        assert(ret == 0);
    }

private:
    pthread_cond_t cond_;
    MutexLock &mutex_;
};

#endif