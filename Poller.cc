#include "Poller.h"
#include "Channel.h"
#include "EventLoop.h"

#include <assert.h>

Poller::Poller(EventLoop *loop)
    :   loop_(loop)
{
    assert(loop_ != NULL);
}

Poller::~Poller() = default;

bool Poller::hasChannel(Channel *channel) const 
{
    assertInLoopThread();

    int fd = channel->fd();
    auto it = fdToChannel_.find(fd);
    if(it != fdToChannel_.end())
    {
        assert(it->second == channel);
        return true;
    }
    return false;
}

void Poller::assertInLoopThread() const
{
    loop_->assertInLoopThread();
}