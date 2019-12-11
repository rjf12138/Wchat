// 这个类要改为 msg 处理的类， server 则是用来服务的启动和消息的传递和管理

#ifndef __SERVER_H__
#define __SERVER_H__

#include "basic_head.h"

class Server {
public:
    Server(string ip, short port, int timeout = 100);
    virtual ~Server(void);

    virtual void process_task(void);
    virtual void run(void);
    virtual void stop(void);

private:
private:
    bool exit_;
};

#endif