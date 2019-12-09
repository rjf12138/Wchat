#ifndef __REACTOR_H__
#define __REACTOR_H__

#include "basic_head.h"
#include "event_handler.h"
#include "thread.h"

#define MAX_EVENT_NUM 512

class Reactor : public Thread {
public:
    Reactor(EventHandler &event_handler);
    virtual ~Reactor(void);

    int init(void);
    int add_input_fd(int fd);
    int add_output_fd(int fd);
    int del_fd(int fd);
    int stop(void);

    virtual int run_handler() override;
    virtual int exit_handler() override;

private:
    int setnonblocking(int fd);

private:
    int epoll_fd_ = -1;
    bool exit_ = false;
    EventHandler &event_hander_;
    struct epoll_event event_[MAX_EVENT_NUM];
};

#endif