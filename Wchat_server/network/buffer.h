/*
*   start_pos 指向第一个可读/写位置， end_pos 指向最后一个可读写之后的位置
*   整个buffer采用循环队列的方式保存数据
*/

#ifndef __BUFFER_H__
#define __BUFFER_H__

#include "basic_head.h"

#define MAX_BUFFER_SIZE 1024
#define MAX_STRING_SIZE 512

class Buffer {
public:
    Buffer();
    virtual ~Buffer();

    int read_int8(int8_t &val);
    int read_int16(int16_t &val);
    int read_int32(int32_t &val);
    int read_int64(int64_t &val);
    int read_string(string &str);
    int read_bytes(void *buf, int buf_size, bool match = false);

    int write_int8(int8_t val);
    int write_int16(int16_t val);
    int write_int32(int32_t val);
    int write_int64(int64_t val);
    int write_string(string str);
    int write_bytes(const void *buf, int buf_size);

    int read_int8_lock(int8_t &val);
    int read_int16_lock(int16_t &val);
    int read_int32_lock(int32_t &val);
    int read_int64_lock(int64_t &val);
    int read_string_lock(string &str);
    int read_bytes_lock(void *buf, int buf_size);

    int write_int8_lock(int8_t val);
    int write_int16_lock(int16_t val);
    int write_int32_lock(int32_t val);
    int write_int64_lock(int64_t val);
    int write_string_lock(string str);
    int write_bytes_lock(const void *buf, int buf_size);

    bool empty(void);
    bool full(void);
    int size(void);
    int clear(void);
    int resize(int size);
private:
    bool check_read_pos(void);
    bool check_write_pos(void);

    void next_read_start_pos(int offset = 1);
    void next_read_end_pos(int offset = 1);
    void next_write_start_pos(int offset = 1);
    void next_write_end_pos(int offset = 1);
private:
    vector<char> write_buffer_;
    vector<char> read_buffer_;

    int start_read_pos_;
    int end_read_pos_;
    int start_write_pos_;
    int end_write_pos_;

    int write_buffer_size_;
    int read_buffer_size_;
};

#endif