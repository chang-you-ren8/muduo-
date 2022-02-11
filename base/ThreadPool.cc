#include "ThreadPool.h"


ThreadPool::ThreadPool(int thread_num, int max_queue_size)
	:thread_num_(thread_num)
	,max_queue_size_(max_queue_size)
	,mutex_()
	,not_empty_(mutex_)
	,not_full_(mutex_)
{
	is_running_ = false;
	assert(thread_num > 0);
	assert(max_queue_size > 0);
}


ThreadPool::~ThreadPool()
{
	if(is_running_)
		stop();
}

ThreadPool::Task ThreadPool::take(void)
{
	MutexLockGuard guard(mutex_);
	while(que_.empty() && is_running_)
		not_empty_.wait();

	Task task;
	if(!que_.empty())
	{
		task = que_.front();
		que_.pop();
	}
	not_full_.notify();
	return task;
}

void ThreadPool::put(Task task)
{
	MutexLockGuard guard(mutex_);
	while(is_full() && is_running_)
		not_full_.wait();
	
	if(!is_running_)
		return;

	que_.push(task);
	not_empty_.notify();
}

void ThreadPool::start()
{
	assert(!is_running_);
	
	is_running_ = true;

	for(int i = 0; i < thread_num_; ++i)
		threads_.emplace_back(new Thread(std::bind(&ThreadPool::run_in_thread, this)));
	
	for(int i = 0; i < thread_num_; ++i)
		threads_[i]->start();
}

//无法保证所有que_中所有任务被执行完毕
void ThreadPool::run_in_thread(void)
{
	while(is_running_)
	{
		Task task = take();
		if(task)
			task();
	}
}


void ThreadPool::stop(void)
{
	//这里必须将mutex_释放了，才能join。否则死锁
	{
		MutexLockGuard guard(mutex_);
		is_running_ = false;
		not_empty_.notifyAll();
		not_full_.notifyAll();
	}

	for(const auto &ptr : threads_)
		ptr->join();
}

