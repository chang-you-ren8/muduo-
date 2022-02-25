#include <sys/resource.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <iostream>
using namespace std;

int main()
{
    struct rlimit rlim;
    bzero(&rlim, sizeof(rlim));
    int ret = getrlimit(RLIMIT_NOFILE, &rlim);
    assert(ret == 0);
    cout << rlim.rlim_cur << " " << rlim.rlim_max << endl;
    return 0;
}