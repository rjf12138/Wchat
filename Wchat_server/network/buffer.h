#ifndef __BUFFER_H__
#define __BUFFER_H__

#include "basic_head.h"

#define MAX_BUFFER_SIZE 1024

class Buffer {
public:
    Buffer();
    virtual ~Buffer();

    int read_int8(int8_t &val);
    int read_int16(int16_t &val);
    int read_int32(int32_t &val);
    int read_int64(int64_t &val);
    int read_string(string &str);
    int read_bytes(void *buf, int buf_size);

    int write_int8(int8_t val);
    int write_int16(int16_t val);
    int write_int32(int32_t val);
    int write_int64(int64_t val);
    int write_string(string str);
    int write_bytes(const void *buf, int buf_size);

    int clear(void);
    int resize(int size);
private:
    vector<char> buffer_;
    int start_read_pos_;
    int end_read_pos_;
    int start_write_pos_;
    int end_write_pos_;
};

#endif