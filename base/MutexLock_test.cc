#include "Thread.h"
#include "MutexLock.h"
#include "MutexLockGuard.h"
#include <stdint.h>
#include <sys/time.h>
#include <iostream>
#include <vector>
#include <memory>

using std::cout;
using std::endl;
using std::vector;
using std::unique_ptr;

MutexLock g_mutex;
vector<int> g_vi;
const int kCount = 1000 * 10000;
const int N = 1e6;

void threadFunc(void)
{
	MutexLockGuard guard(g_mutex);
	for(int i = 0; i < kCount; ++i)
	{
		if(g_vi.size() >= kCount)
			return;
		g_vi.push_back(i);
	}
}

int64_t now()
{
	struct timeval tim;
	gettimeofday(&tim, NULL);

	int64_t ret = tim.tv_sec * N + tim.tv_usec;
	return ret;
}

void test(int thread_num)
{
	g_vi.clear();

	int64_t st = now();

	vector<unique_ptr<Thread>> vptr;

	for(int i = 0; i < thread_num; ++i)
		vptr.emplace_back(new Thread(threadFunc));
	
	for(int i = 0; i < thread_num; ++i)
		vptr[i]->start();
	
	for(int i = 0; i < thread_num; ++i)
		vptr[i]->join();
	
	int64_t en = now();
	cout << "use " << (en - st) << " us\n";
}

int main(void)
{
	g_vi.resize(kCount);

	test(100);
	test(10);
	test(5);
	test(4);
	test(3);
	test(2);

	return 0;
}
