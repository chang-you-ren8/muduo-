#include "Buffer.h"

#include <errno.h>
#include <unistd.h>

void Buffer::makeSpace(size_t len)
{
    //prependableBytes()就是readerIndex_
    //readerIndex_ - kCheapPrepend + writableBytes() 就是buffer_的空闲的内存大小
    if(writableBytes() + prependableBytes() < len + kCheapPrepend)
    {
        buffer_.resize(writerIndex_ + len);
    }
    else  
    {
        //移动
        size_t readable = readableBytes();
        //内存地址重叠：高地址拷贝到低地址没问题，但是低地址拷贝到高地址会产生问题。
        //bug
        std::copy(begin() + readerIndex_,
                begin() + writerIndex_,
                begin() + kCheapPrepend);
        readerIndex_ = kCheapPrepend;
        writerIndex_ = kCheapPrepend + readable;
    }
}

//这里使用了两个缓冲区
//1.buffer_的空闲区
//2.栈上的一个空间
ssize_t Buffer::readFd(int fd, int *savedErrno)
{
    char extraBuf[65536];
    struct iovec vec[2];
    const size_t writable = writableBytes();
    vec[0].iov_base = begin() + writerIndex_;
    vec[0].iov_len = writable;
    vec[1].iov_base = extraBuf;
    vec[1].iov_len = sizeof extraBuf;

    //每次至多读取128k
    const int iovcnt = (writable < sizeof extraBuf ? 2 : 1);
    const ssize_t n = sockets::readv(fd, vec, iovcnt);
    if(n < 0)
    {
        *savedErrno = errno;
    }
    else if(implicit_cast<size_t>(n) <= writable)
    {
        writerIndex_ += n;
    }
    else 
    {
        writerIndex_ = buffer_.size();
        append(extraBuf, n - writable);
    }

    return n;
}