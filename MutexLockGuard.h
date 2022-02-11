#ifndef BASE_MUTEXLOCKGUARD_H
#define BASE_MUTEXLOCKGUARD_H

#include "noncopyable.h"
#include "MutexLock.h"

//RAII手法来确保mutex一定会被解锁
class MutexLockGuard:public noncopyable
{
public:
	MutexLockGuard(MutexLock &mutex)
		:mutex_(mutex)
	{
		mutex_.lock();
	}

	~MutexLockGuard()
	{
		mutex_.unlock();
	}

private:
	MutexLock &mutex_;
};
#endif