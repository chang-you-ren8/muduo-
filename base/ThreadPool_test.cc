#include "ThreadPool.h"
#include <iostream>
#include <functional>
#include <string>
using std::cout;
using std::endl;
using std::string;


void print(string str)
{
	for(int i = 0; i < 10; ++i)
		cout << str << endl;
}

int main(void)
{	
	//线程数量，队列大小
	ThreadPool pool(3, 10000);
	pool.start();

	pool.put(std::bind(print, "hello world"));
	pool.put(std::bind(print, "my threadpool"));

	pool.stop();

	return 0;
}
