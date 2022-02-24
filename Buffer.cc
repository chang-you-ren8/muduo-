#include "Buffer.h"

#include <sys/uio.h>

//每次至多读取64kb
//非阻塞IO只需要读取一次即可
ssize_t Buffer::readFd(int fd, int *savedErrno)
{
    char extrabuf[65536];

    const size_t writeable = writeableBytes();
    struct iovec vec[2];
    vec[0].iov_base = writerBegin();
    vec[0].iov_len = writeable;
    vec[1].iov_base = extrabuf;
    vec[1].iov_len = sizeof(extrabuf);

    int iov_cnt = (writeable >= sizeof(extrabuf) ? 1 : 2);
    ssize_t n = ::readv(fd, vec, iov_cnt);
    if(n == -1)
    {
        *savedErrno = errno;
    }
    else if(static_cast<size_t>(n) <= writeable)
    {
        writerIndex_ += n;
    }
    else    
    {
        writerIndex_ += writeable;
        append(extrabuf, n - writeable);
    }

    return n;
}