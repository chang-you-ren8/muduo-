#include "Thread.h"

#include <iostream>

using namespace std;

class Test
{
public:
    Test()
        : thread_(bind(&Test::print, this))
    {

    }

    ~Test() 
    {
        thread_.join();
    }

    void start()
    {
        thread_.start();
    }

private:
    Thread thread_;

    void print()
    {
        cout  << "hello world" << endl;
    }
};

int main(void)
{
    Test test;
    test.start();
    return 0;
}