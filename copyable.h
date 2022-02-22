#ifndef BASE_COPYABLE_H
#define BASE_COPYABLE_H

//这个copyable类为什么不指明拷贝构造呢？
class copyable
{
protected:
    copyable() = default;
    ~copyable() = default;
};

#endif 