#ifndef __MESSAGE_H__
#define __MESSAGE_H__

/*
*   消息的基本结构(头部)
*   magic_num   (2个字节(用 @$， 其它地方要用到这两个字符需要进行替代))
*   消息类型    (1个字节）
*   消息体长度   (2个字节[取决于消息体的长度])
*   消息体      (未知)
*   校验        (待定)
*/

/*
*   消息体解析取决于具体的消息类
*/
#include "basic_head.h"

class MessageBase {
public:
    MessageBase() {}
    virtual ~MessageBase() {}

    int get_msg_type(void) const {return msg_type_;}

private:
    // 基础的解包，主要为了获取消息类型，好生成对应的消息结构
    int get_msg_type_from_buffer(void);

private:
    int msg_type_;
};

// 为这个服务器定义各种需要处理的消息结构
// 例：
// struct EchoMessage : public MessageBase {
//     string msg_body;
// };

////////////////////// Inner Msg Struct //////////////////////////

#include "inner_msg_object.h"

struct InnerMsg {
    int msg_id_;
    OBJ_HANDLE handler_;
    int param1_;
    string str_;
    shared_ptr<void> object_;

    InnerMsg(int msg_id = 0, OBJ_HANDLE handler = 0, int param1 = 0, string str = "", void* object = nullptr):
        msg_id_(msg_id), handler_(handler), param1_(param1), str_(str), object_(object)
    {}
};


#endif