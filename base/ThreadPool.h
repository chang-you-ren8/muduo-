#ifndef _BASE_THREADPOOL_H_
#define _BASE_THREADPOOL_H_

#include "noncopyable.h"
#include "MutexLock.h"
#include "MutexLockGuard.h"
#include "Condition.h"
#include "Thread.h"
#include <functional>
#include <memory>
#include <queue>

class ThreadPool:public noncopyable
{
public:
	using Task = std::function<void(void)>;

	ThreadPool(int thread_num, int max_queue_size);
	~ThreadPool();

	Task take(void);
	void put(Task task);

	void start(void);
	void stop(void);

private:
	const int thread_num_;
	const int max_queue_size_;
	std::queue<Task> que_;
	MutexLock mutex_;	
	Condition not_empty_;
	Condition not_full_;
	std::vector<std::unique_ptr<Thread>> threads_;

	bool is_running_;

	bool is_full() const
	{
		return static_cast<int>(que_.size()) == max_queue_size_;
	}

	void run_in_thread(void);
};

#endif

