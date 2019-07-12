#ifndef __SEND_MSG_H__
#define __SEND_MSG_H__

#include "fragment_operate.h"
#include "msg_fragment.h"
#include "msg_type.h"

class MsgUtil {
public: 
    static struct fragment generate_fragment(uint8_t p_msg_type, std::string data1 = "", std::string data2 = "");
    static std::string print_msg_type(uint8_t p_msg_type);
    static void print_fragment(const struct fragment& p_frag);
};

#endif