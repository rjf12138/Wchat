#include "reactor.h"

Reactor::Reactor(/*...未定参数*/) { }
Reactor::~Reactor(void) { }

int Reactor::init(void)
{
    // 5 只是提示内核表要多大，并不起作用
    epoll_fd_ = epoll_create(5);
    return epoll_fd_;
}
int Reactor::wait(void)
{
    while(!exit_) {
        int ret = epoll_wait(epoll_fd_, &event_, MAX_EVENT_NUM, );
    }
}
int Reactor::contrl(int fd, int op);
int Reactor::handler(int fd);
int Reactor::stop(void);