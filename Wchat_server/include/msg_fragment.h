#ifndef __MSG_FRAGMENT_H__
#define __MSG_FRAGMENT_H__

#include <string>
#include <cstring>
#include <cstdio>

#define UNIT_DATA_BUFFER_BYTE       255
#define MAX_FRAGMENT_NUMBER          5
#define MAX_BUCKET_NUMBER           20
#define IDENTITY_LENGTH             32

#define MAX_ACCOUNT_LENGTH          32

struct fragment {
    char msg_type;                               // 消息类型
    char sender_account[MAX_ACCOUNT_LENGTH];     // 发送者账号
    char receiver_account[MAX_ACCOUNT_LENGTH];   // 接受者账号
    char group_account[MAX_ACCOUNT_LENGTH];      // 群账号
    char fragment_amount;                        // 分片数量
    char fragment_position;                      // 分片在消息中的位置
    char fragment_length;                        // 该分片的长度
    uint tmp;                                    // 可以临时使用的字段
    char identity[IDENTITY_LENGTH];              // 该分片的唯一标识。假设格式：用户账号+4个字节可变字符数字
    char data[UNIT_DATA_BUFFER_BYTE];            // 传输数据
};

#endif