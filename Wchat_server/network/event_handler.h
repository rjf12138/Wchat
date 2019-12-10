/*
*   处理事件的响应，将数据保存到对应的fd中, 且不管数据只将数据向上传
*/

#ifndef __EVENT_HANDLER_H__
#define __EVENT_HANDLER_H__

#include "basic_head.h"
#include "reactor.h"
#include "buffer.h"

struct FdBuffer {
    FdBuffer(int fd, int fd_type) :fd_(fd), fd_type_(fd_type) {}
    int fd_;
    int fd_type_;
    Buffer buffer_;
};

class EventHandler {
public:
    EventHandler(void): reactor_(*this) {}
    virtual ~EventHandler(void) {}

    virtual int add_input_fd(int fd, int fd_type) 
    { 
        FdBuffer tmp_fd(fd, fd_type);
        io_buffer_[fd] = tmp_fd;
        reactor_.add_input_fd(fd); 
    }
    
    virtual int handler_event(int fd);

private:
    Reactor reactor_;
    map<int, FdBuffer> io_buffer_;
};

#endif