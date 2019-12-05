#ifndef __REACTOR_H__
#define __REACTOR_H__

#include "basic_head.h"
#include "thread.h"

#define MAX_EVENT_NUM 512

class Reactor : public Thread {
public:
    Reactor(/*...未定参数*/);
    virtual ~Reactor(void);

    virtual int handler(int fd);

    int init(void);
    int wait(void);
    int add_fd(int fd);
    int mod_fd(int fd);
    int del_fd(int fd);
    int stop(void);

private:
    int setnonblocking(int fd);
private:
    int epoll_fd_;
    struct epoll_event event_[MAX_EVENT_NUM];
    bool exit_ = false;
};

#endif