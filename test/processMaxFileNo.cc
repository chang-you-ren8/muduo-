#include <sys/socket.h>
#include <sys/resource.h>
#include <errno.h>
#include <assert.h>

#include <iostream>
using namespace std;


int main()
{
    struct rlimit rlim;
    int ret = getrlimit(RLIMIT_NOFILE, &rlim);
    assert(ret == 0);
    cout << "process can create file number : soft_max = " << rlim.rlim_cur;
    cout << " hard_max = " << rlim.rlim_max << endl;

    rlim.rlim_cur += 4096;
    ret = setrlimit(RLIMIT_NOFILE, &rlim);
    assert(ret == 0);

    ret = getrlimit(RLIMIT_NOFILE, &rlim);
    assert(ret == 0);
    cout << "process can create file number : soft_max = " << rlim.rlim_cur;
    cout << " hard_max = " << rlim.rlim_max << endl;


    return 0;
}