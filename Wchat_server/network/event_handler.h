#ifndef __EVENT_HANDLER_H__
#define __EVENT_HANDLER_H__

#include "basic_head.h"
#include "reactor.h"
#include "buffer.h"

class EventHandler {
public:
    EventHandler(void): reactor_(*this) {}
    virtual ~EventHandler(void) {}

    virtual int handler_event(int fd);
private:
    Reactor reactor_;
    map<int, Buffer> io_buffer_;
};

#endif