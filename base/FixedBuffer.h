#ifndef BASE_FIXEDBUFFER_H
#define BASE_FIXEDBUFFER_H 

#include "noncopyable.h"

#include <string.h>

template<int SIZE>
class FixedBuffer : noncopyable
{
public:
    FixedBuffer()
        : cur_(data_)
    { }    

    void append(const char *msg, int len)
    {
        if(avail() >= len)
        {
            memcpy(cur_, msg, len);
            cur_ += len;
        }
    }

    int avail() const { return static_cast<int>(SIZE - length()); } 

    int length() const { return static_cast<int>(cur_ - data_); }
    int size() const {return length(); }

    void add(int siz) { cur_ += siz; }
    void reset() { cur_ = data_; }
    void bzero() { ::bzero(data_, sizeof(data_)); }

    char *current() { return cur_; } 
    const char *data() const { return data_; }

private:
    char data_[SIZE];
    char *cur_; //指向第一个未使用
};

const int kBigFixedBufferSize = 1024 * 1024;
const int kSmallFixedBufferSize = 64 * 1024;
using BigFixedBuffer = FixedBuffer<kBigFixedBufferSize>;
using SmallFixedBuffer = FixedBuffer<kSmallFixedBufferSize>;

#endif 