#ifndef NET_BUFFER_H
#define NET_BUFFER_H

#include "copyable.h"

#include <assert.h>

#include <vector>
#include <string>
#include <algorithm>

using std::string;

//值语义
class Buffer : copyable
{
public:
    static const size_t kCheapPrepend = 8;
    static const size_t kInitialSize = 1024;

    explicit Buffer(size_t initialSize = kInitialSize)
        :   buffer_(kCheapPrepend + initialSize),
            readerIndex_(kCheapPrepend),
            writerIndex_(kCheapPrepend)
    { }

    size_t readableBytes() const   
    { return writerIndex_ - readerIndex_; }

    //writableBytes ???
    size_t writeableBytes() const  
    { return buffer_.size() - writerIndex_; }

    bool empty() const   
    { return readableBytes() == 0; }

    char *readerBegin()
    { return &buffer_[readerIndex_]; }

    char *writerBegin()
    { return &buffer_[writerIndex_]; }

    void retrieveAll()
    { readerIndex_ = writerIndex_ = kCheapPrepend; }

    void retrieve(size_t n)
    {
        assert(n <= readableBytes());

        if(n == readableBytes())
        {
            retrieveAll();
        }
        else   
        {
            readerIndex_ += n;
        }
    }

    string retrieveAllAsString()
    {
        string str(readerBegin(), readableBytes());
        retrieveAll();
        return str;
    }    

    string retrieveAsString(size_t n)
    {
        if(n == readableBytes())
        {
            return retrieveAllAsString();
        }

        string str(readerBegin(), n);
        retrieve(n);
        return str;
    }

    void append(const string &data)
    { append(data.c_str(), data.size()); }

    void append(const char *data, size_t len)
    {
        if(writeableBytes() < len)
            ensureBytes(len);
        std::copy(data, data + len, writerBegin());
        writerIndex_ += len;
    }
    
    //从文件描述符fd中读取数据到内部缓冲区
    ssize_t readFd(int fd, int *savedErrno);

private:
    std::vector<char> buffer_;
    size_t readerIndex_;            //第一个可读字节所处的位置
    size_t writerIndex_;            //第一个可写位置

    void ensureBytes(size_t len)
    {
        //将数据从后面移动到从kCheapPerpend开始的地方
        size_t readable = readableBytes();
        size_t needed = kCheapPrepend + readable + len;
        if(needed <= buffer_.size())
        {
            std::copy(readerBegin() , readerBegin() + readable, buffer_.begin() + kCheapPrepend);
            readerIndex_ = kCheapPrepend;
            writerIndex_ = kCheapPrepend +  readable;
        }
        else   
        {
            //使用index的话readerIndex_, writerIndex_不会失效。如果使用迭代器就可能失效
            buffer_.resize(needed);
        }
    }

};

#endif
