#include "SingleInstance.h"
#include "Thread.h"

#include <pthread.h>

#include <functional>
#include <iostream>

using namespace std;

class Single
{
public:
	Single()
	{
		cout << "in Single ctor\n";
	}

	~Single()
	{
		cout << "in Single dtor\n";
	}

};

void run(void *arg)
{
	Single *single = SingleInstance<Single>::instance();
	(void)single;
}

int main(void)
{
	Thread t1(std::bind(run, nullptr));
	t1.start();

	Thread t2(std::bind(run, nullptr));
	t2.start();

	t1.join();
	t2.join();

	return 0;
}
