#include "Poller.h"

Poller::Poller(EventLoop *loop)
    :loop_(loop)
{

}

Poller::~Poller() = default;


bool Poller::hasChannel(Channel *channel) const
{
    auto it = fdToChannel_.find(channel->fd());
    if(it != fdToChannel_.end() && it->second == channel)
        return true;
    return false;
}

void Poller::assertInLoopThread()
{
    loop_->assertInLoopThread();
}