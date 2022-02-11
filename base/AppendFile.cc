#include "AppendFile.h"
#include "Logger.h"

//a--添加模式（只写），不存在则创建这个文件
//e--CLOEXEC
AppendFile::AppendFile(const char *filename)
    : fp_(::fopen(filename, "ae")),
      writtenBytes_(0)   
{

}

AppendFile::~AppendFile()
{
    ::fclose(fp_);
}


//文件流上面的写入错误通过ferror来判断
void AppendFile::append(const char *msg, size_t len)
{
    size_t remain = len;
    while(remain > 0)
    {
        size_t n = write(msg + (len - remain), remain);
        if(n != remain)
        {
            if(::ferror(fp_))
            {
                LOG_ERROR << "AppendFile::append";
                break;
            }
        }
        remain -= n;
        writtenBytes_ += n;
    }
}

void AppendFile::flush()
{
    ::fflush(fp_);
}

//非线程安全
size_t AppendFile::write(const char *msg, size_t len)
{
    return fwrite_unlocked(msg, sizeof(char), len, fp_);
}