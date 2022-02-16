#include "Thread.h"

#include <unistd.h>
#include <stdio.h>

#include <vector>
#include <functional>
#include <memory>
#include <iostream>


using namespace std;

void print()
{
    sleep(100);
}

int main(void)
{
    //pstree -p pid 可以查看进程pid创建的所有线程
    printf("main thread pid = %d\n", (int)getpid());

    vector<unique_ptr<Thread>> threads;
    int n = 5;
    for(int i = 0; i < n; ++i)
        threads.emplace_back(new Thread(bind(print)));

    for(int i = 0; i < n; ++i)
        threads[i]->start();

    //如何验证？使用ps或者top查看???带完成
    for(int i = 0; i < n; ++i)    
        printf("%d\n", (int)threads[i]->tid());

    for(int i = 0; i < n; ++i)
        threads[i]->join();

    return 0;
}