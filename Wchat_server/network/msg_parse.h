#ifndef __HANDLE_MESSAGE_H__
#define __HANDLE_MESSAGE_H__
/*
*   等待从event_handle上来的数据，根据message里的类型转为具体
*   的结构上传到server上
*/
#include "basic_head.h"
#include "message.h"
#include "thread.h"

class MessageHandler : public Thread, public MSGObject{
public:
    MessageHandler(void);
    virtual ~MessageHandler(void);

    virtual int run_handler(void) override;
    virtual int exit_handler(void) override;

    virtual int on_msg(InnerMsg msg) override;
private:
    map<int, Queue<Buffer>> io_buffer_;
};


#endif