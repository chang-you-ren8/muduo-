#include "Channel.h"

#include <assert.h>

Channel::Channel(EventLoop *loop, int fd)
    :   loop_(loop),
        fd_(fd),
        index_(-1),
        eventHandling_(false),
        tied_(false),
        events_(0),
        revents_(0)
{

}

Channel::~Channel()
{
    assert(!eventHandling_);
}

void Channel::tie(const std::shared_ptr<void> &obj)
{
    tied_ = true;
    tie_ = obj;
}

void Channel::handleEvent(Timestamp recieveTime)
{
    eventHandling_ = true;

    //确保Channel的生命周期长于handleEvent
    std::shared_ptr<void> guard;
    if(tied_)
        guard = tie_.lock();

    if(revents_ & (POLLIN | POLLRDHUP))
    {
        if(readCallback_)
            readCallback_(recieveTime);
    }

    if(revents_ & POLLOUT)
    {
        if(writeCallback_)
            writeCallback_();
    }

    if(revents_ & POLLERR)
    {
        if(errorCallback_)
            errorCallback_();
    }

    eventHandling_ = false;
}