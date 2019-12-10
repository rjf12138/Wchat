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

// 
struct MessageBase {
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

#endif