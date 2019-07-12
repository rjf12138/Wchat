#ifndef __FRAGMENT_OPERATE__
#define __FRAGMENT_OPERATE__

#include <ctime>
#include <string>

#include "queue.h"
#include "msg_buffer.h"
#include "msg_fragment.h"


class FragmentOperate{
public:
    static void set_msg_type(struct fragment& frag, uint8_t type) {frag.msg_type = type;}                                   // 设置消息类型
    static void get_msg_type(const struct fragment& frag, uint8_t &type) {type = frag.msg_type;}                            // 获取消息类型

    static void set_sender_account(struct fragment& frag, std::string account)                                              // 设置发送者账号
                {memset(frag.sender_account, 0, MAX_ACCOUNT_LENGTH); sprintf(frag.sender_account, "%s", account.c_str());}
    static void get_sender_account(const struct fragment& frag, std::string &account)                                       // 获取发送者账号
                {account = std::string(frag.sender_account);}

    static void set_receiver_account(struct fragment& frag, std::string account)                                            // 设置接受者账号
                {memset(frag.receiver_account, 0, MAX_ACCOUNT_LENGTH); sprintf(frag.receiver_account, "%s", account.c_str());}
    static void get_receiver_account(const struct fragment& frag, std::string& account)                                     // 获取接受者账号
                {account = std::string(frag.receiver_account);}
    
    static void set_group_account(struct fragment& frag, std::string account)                                               // 设置群账号
                {memset(frag.group_account, 0, MAX_ACCOUNT_LENGTH); sprintf(frag.group_account, "%s", account.c_str());}
    static void get_group_account(const struct fragment& frag, std::string &account)                                        // 获取群账号
                {account = std::string(frag.group_account);}

    static void set_fragment_amount(struct fragment& frag, uint8_t amount) {frag.fragment_amount = amount;}                 // 设置消息的分片数量
    static void get_fragment_amount(const struct fragment& frag, uint8_t& amount) {amount = frag.fragment_amount;}          // 获取消息的分片数量

    static void set_fragment_position(struct fragment& frag, uint8_t position) {frag.fragment_amount = position;}           // 设置该分片在整个消息中的位置
    static void get_fragment_position(const struct fragment& frag, uint8_t& position) {position = frag.fragment_position;}  // 获取该分片在整个消息中的位置

    static void set_fragment_length(struct fragment& frag, uint8_t length) {frag.fragment_length = length;}                 // 设置分片中消息的长度
    static void get_fragment_length(const struct fragment& frag, uint8_t& length) {length = frag.fragment_length;}          // 获取分片中消息的长度

    static void set_identity(struct fragment& frag, std::string identity)                                                   // 设置消息的身份标志
                {memset(frag.identity, 0, IDENTITY_LENGTH); sprintf(frag.identity, "%s", identity.c_str());}
    static void get_identity(const struct fragment& frag, std::string& identity)                                            // 获取消息的身份标志
                {identity = std::string(frag.identity);}
    static void create_a_unique_identity(char *identity, int size)                                                          // 服务端创建消息的唯一标志
    {
        if (identity == nullptr) {
            return ;
        }
        time_t identity_tmp = time(nullptr);
        memset(identity, 0, size);
        snprintf(identity, size, "%ld", identity_tmp);
    }

    static void set_and_push_data_to_queue(struct fragment& frag, const std::string& data, int is_online)           // 将消息压入队列中，如果消息过大就放入分片中
    {
        create_a_unique_identity(frag.identity, IDENTITY_LENGTH);
        uint8_t frag_amount = data.size() / UNIT_DATA_BUFFER_BYTE + 1;
        set_fragment_amount(frag, frag_amount);
        for (int i = 1; i <= frag_amount; ++i) {
            int start_copy_pos = (i-1) * UNIT_DATA_BUFFER_BYTE;
            if (i * UNIT_DATA_BUFFER_BYTE < data.size()) {
                set_fragment_position(frag, i);
                set_fragment_length(frag, UNIT_DATA_BUFFER_BYTE);
                memset(frag.data, 0, UNIT_DATA_BUFFER_BYTE);
                snprintf(frag.data, UNIT_DATA_BUFFER_BYTE, "%s", data.c_str() + start_copy_pos);
            }  else {
                int msg_length = data.size() - (i-1) * UNIT_DATA_BUFFER_BYTE;
                set_fragment_position(frag, i);
                set_fragment_length(frag, msg_length);
                snprintf(frag.data, msg_length+1, "%s", data.c_str() + start_copy_pos);
            }
            if (is_online) {
                FRAGMENT_SEND_PUSH(frag);
            } else {
                MSG_BUFFER_PUSH(frag);
            }
        }
    }

    static void push_fragment_to_queue(struct fragment& frag, int is_online)           // 设置分片的基本信息后，将消息放到队列中
    {
        create_a_unique_identity(frag.identity, IDENTITY_LENGTH);
        set_fragment_position(frag, 1);
        set_fragment_amount(frag, 1);
        set_fragment_length(frag, 0);
        if (is_online) {
            FRAGMENT_SEND_PUSH(frag);
        } else {
            MSG_BUFFER_PUSH(frag);
        }
    }

    static void transmit_fragment(struct fragment& frag, int is_online)                 // 不修改分片直接转发消息
    {
        if (is_online) {
            FRAGMENT_SEND_PUSH(frag);
        } else {
            MSG_BUFFER_PUSH(frag);
        }
    }
};


#endif