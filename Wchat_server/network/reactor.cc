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
        int ret = epoll_wait(epoll_fd_, event_, MAX_EVENT_NUM, 5000);
        if (ret == -1) {
            sleep(1);
            continue;
        }

        for (int i = 0; i < MAX_EVENT_NUM; ++i) {
            this->handler(event_[i].data.fd);
        }
    }
}

int Reactor::add_fd(int fd)
{
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &event);
    setnonblocking(fd);

    return 0;
}

int Reactor::mod_fd(int fd) { }

int Reactor::del_fd(int fd) { }

int Reactor::handler(int fd) { }

int Reactor::stop(void)
{
    exit_ = true;
}

int Reactor::setnonblocking(int fd)
{
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}