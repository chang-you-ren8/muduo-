#ifndef NET_BUFFER_H
#define NET_BUFFER_H

#include "SocketOps.h"

#include <string.h>
#include <stdint.h>
#include <assert.h>

#include <vector>
#include <string>
#include <algorithm>

//说白了这个缓冲区就是vector<char>加上writerIndex_和readerIndex_
//writerIndex_和readerIndex_都是正数，不是迭代器（防止内存重新分配后迭代器失效）
//至于为什么开头留了8个字节，我不清楚

//内部使用vector<char>避免手动管理内存
//只是简单的移动（不是环形缓冲区，但是可以保证缓冲区的连续性)
//value 语义
//Buffer存储的是网络字节序数据
class Buffer
{
public:
    static const size_t kCheapPrepend = 8;
    static const size_t kInitalSize = 1024;

    //[readerIndex_, writerIndex_) 这段区间可读
    //[writerIndex_, buffer_.size())这段区间可写
    explicit Buffer(size_t initalSize = kInitalSize)
        :   buffer_(kCheapPrepend + initalSize),
            readerIndex_(kCheapPrepend),
            writerIndex_(kCheapPrepend)
        { }

    void swap(Buffer &rhs)
    {
        buffer_.swap(rhs.buffer_);
        std::swap(readerIndex_, rhs.readerIndex_);
        std::swap(writerIndex_, rhs.writerIndex_);
    }

    size_t readableBytes() const
    { return writerIndex_ - readerIndex_; }

    size_t writableBytes() const  
    { return buffer_.size() - writerIndex_; }

    size_t prependableBytes() const 
    { return readerIndex_; }

    const char *peek() const 
    { return begin() + readerIndex_; }

    void retrieve(size_t len)
    {
        assert(len <= readableBytes());
        if(len < readableBytes())
            readerIndex_ += len;
        else  
            retrieveAll();
    }

    void retrieveAll()
    {
        readerIndex_ = kCheapPrepend;
        writerIndex_ = kCheapPrepend;
    }

    void retrieveInt64()
    { retrieve(sizeof(int64_t)); }

    void retrieveInt32()
    { retrieve(sizeof(int32_t)); }

    void retrieveInt16()
    { retrieve(sizeof(int16_t)); }

    std::string  retrieveAllAsString()
    { return retrieveAsString(readableBytes()); }

    std::string retrieveAsString(size_t len)
    {
        assert(len <= readableBytes());
        string ret(peek(), len);
        retrieve(len);
        return ret;
    }

    void append(const std::string &data)
    { append(data.c_str(), data.size()); }

    void append(const char *data, size_t len)
    {
        ensureWritableBytes(len);
        std::copy(data, data + len, beginWrite());
        hasWritten(len);
    }

    void append(const void *data, size_t len)
    { append(static_cast<const char *>(data), len); }

    void ensureWritableBytes(size_t len)
    {
        if(writableBytes() < len)
            makeSpace(len);
        assert(writableBytes() >= len);
    }

    char *beginWrite()
    { return begin() + writerIndex_; }
    
    const char *beginWrite() const 
    { return begin() + writerIndex_; }

    void hasWritten(size_t len)
    {
        assert(len <= writableBytes());
        writerIndex_ += len;
    }

    void appendInt64(int64_t x)
    {
        //先转换成网络字节序
        int64_t be64 = sockets::hostToNetwork64(x);
        append(&be64, sizeof be64);
    }

    void appendInt32(int32_t x)
    {
        int32_t be32 = sockets::hostToNetwork32(x);
        append(&be32, sizeof be32);
    }

    void appendInt16(int16_t x)
    {
        int16_t be16 = sockets::hostToNetwork16(x);
        append(&be16, sizeof be16);
    }

    //将fd中的数据读入buffer
    ssize_t readFd(int fd, int *saveErrno);
private:

    char *begin()
    { return &*buffer_.begin(); }

    const char *begin() const 
    { return &*buffer_.begin(); }

    void makeSpace(size_t len);

    std::vector<char> buffer_;
    size_t readerIndex_;        //第一个可读位置
    size_t writerIndex_;        //第一个可写位置
};

#endif 