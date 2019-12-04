#ifndef __REACTOR_H__
#define __REACTOR_H__

#include "basic_head.h"
#include "thread.h"

#define MAX_EVENT_NUM 512

class Reactor : public Thread {
public:
    Reactor(/*...未定参数*/);
    virtual ~Reactor(void);

    virtual int init(void);
    virtual int wait(void);
    virtual int contrl(int fd, int op);
    virtual int handler(int fd);
    virtual int stop(void);

private:
    int epoll_fd_;
    struct epoll_event event_;
    bool exit_ = false;
};

#endif