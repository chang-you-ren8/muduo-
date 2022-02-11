#include "Thread.h"
#include <unistd.h>
#include <iostream>
#include <functional>

using namespace std;

void print(int a)
{
	while(true)
	{
		cout << a << endl;
		a += 1;
		sleep(1);
	}
}

int main(void)
{
	Thread t(bind(print, 3));
	t.start();
	t.join();
	return 0;
}
