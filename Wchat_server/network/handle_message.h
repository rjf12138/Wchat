#ifndef __HANDLE_MESSAGE_H__
#define __HANDLE_MESSAGE_H__
/*
*   等待从event_handle上来的数据，根据message里的类型转为具体
*   的结构上传到server上
*/
#include "basic_head.h"
#include "message.h"
#include "thread.h"
#include "event_handler.h"

class HandleMessage : public Thread {
public:
private:
};


#endif