#ifndef __REACTOR_H__
#define __REACTOR_H__

#include "basic_head.h"
#include "thread.h"

class Reactor : public Thread {
public:
    Reactor(/*...未定参数*/);
    virtual ~Reactor(void);
private:
};

#endif