#include "Thread.h"

#include <stdio.h>

#include <vector>
#include <functional>
#include <memory>
#include <iostream>


using namespace std;

void print()
{
    cout << "hello world" << endl;
}

int main(void)
{
    vector<unique_ptr<Thread>> threads;
    int n = 5;
    for(int i = 0; i < n; ++i)
        threads.empalce_back(new Thread(bind(print)));

    for(int i = 0; i < n; ++i)
        threads[i]->start();

    for(int i = 0; i < n; ++i)    
        printf("%d\n", (int)threads[i]->tid());

    for(int i = 0; i < n; ++i)
        threads[i]->join();

    return 0;
}