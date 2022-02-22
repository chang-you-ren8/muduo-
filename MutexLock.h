#ifndef BASE_MUTEXLOCK_H
#define BASE_MUTEXLOCK_H
//使用RAII封装pthread_mutex

#include "noncopyable.h"

#include <pthread.h>
#include <assert.h>

class MutexLock : noncopyable
{
public:
    MutexLock()
    {
        //不能assert(pthread_mutex_init(&mutex_, NULL) == 0),否则设置NDEBUG时，就完蛋了。。。
        int ret = pthread_mutex_init(&mutex_, NULL);
        assert(ret == 0);
    }

    ~MutexLock()
    {
        int ret = pthread_mutex_destroy(&mutex_);
        assert(ret == 0);
    }

    //应用使用MutexLockGuard而不是手动lock, unlock
    void lock()
    {
        int ret = pthread_mutex_lock(&mutex_);
        assert(ret == 0);
    }

    void unlock()
    {
        int ret = pthread_mutex_unlock(&mutex_);
        assert(ret == 0);
    }

    pthread_mutex_t *getPthreadMutex()
    {
        return &mutex_;
    }

private:
    pthread_mutex_t mutex_;
};

#endif
