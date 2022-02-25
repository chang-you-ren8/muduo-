#include "EventLoop.h"
#include "Timestamp.h"
#include "CurrentThread.h"
#include "EventLoopThread.h"

#include <stdio.h>
#include <unistd.h>

#include <functional>


void  print(int id)
{
	printf("current thread tid = %d\n", CurrentThread::tid());

    printf("current timerId = %d time is %s\n", 
            id,
            Timestamp::now().toDate().c_str());
}

void test()
{
	print(0);

	EventLoop loop;
	loop.runAfter(1.0, std::bind(&print, 1));
	loop.runAfter(2.0, std::bind(&print, 2));
	loop.runAfter(3.0, std::bind(&print, 3));

	loop.runEvery(4.0, std::bind(&print, 4));
	
	loop.loop();
}

void test1()
{
	print(0);

	EventLoopThread thread;
	EventLoop *ioLoop = thread.startLoop();

	ioLoop->runAfter(1.0, std::bind(&print, 1));
	ioLoop->runAfter(2.0, std::bind(&print, 2));
	ioLoop->runAfter(3.0, std::bind(&print, 3));

	auto timerId4 = ioLoop->runEvery(4.0, std::bind(&print, 4));

	sleep(60);
    ioLoop->cancel(timerId4);
    sleep(60);
	//Join????
}

int main()
{
	test1();

	return 0;
}
