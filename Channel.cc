#include "Channel.h"
#include "EventLoop.h"

#include <poll.h>

const int Channel::kNoneEvent = 0;
const int Channel::kWriteEvent = POLLOUT;
const int Channel::kReadEvent = POLLIN;

Channel::Channel(EventLoop *loop, int fd)
    :   loop_(loop),
        fd_(fd),
        events_(0),
        revents_(0),
        tied_(false),
        eventHandling_(false)
{
    assert(loop != NULL);
}

void Channel::update()
{
    loop_->updateChannel(this);
}

void Channel::remove()
{
    loop_->removeChannel(this);
}

void Channel::enableRead()
{
    events_ |= kReadEvent;
    update();
}

void Channel::enableWrite()
{
    events_ |= kWriteEvent;
    update();
}

void Channel::disableRead()
{
    events_ &= ~kReadEvent;
    update();
}

void Channel::disableWrite()
{
    events &= ~kWriteEvent;
    update();
}

void Channel::disableAll()
{
    events_ = kNoneEvent;
    update();
}

void Channel::tie(const shared_ptr<void> &obj)
{
    tied_ = true;
    tie_ = obj;
}

void Channel::handleEvent(Timestamp recieveTime)
{
    eventHandling_ = true;

    std::shared_ptr<void> guard;
    if(tied_)
    {
        guard = tie_.lock();
    }

    if(revents_ & (POLLIN | POLLPRI | POLLRDHUP))
    {
        if(readCallback_)
            readCallback_();
    }

    if(revents_ & POLLOUT)
    {
        if(writeCallback_)
            writeCallback_();
    }

    if(revents_ & (POLLERR | POLLNVAL))
    {
        if(errorCallback_)
            errorCallback_();
    }

    eventHandling_ = false;
}