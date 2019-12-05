#include "buffer.h"

Buffer::Buffer()
{
    buffer_.reserve(MAX_BUFFER_SIZE);
    this->clear();
}
Buffer::~Buffer()
{
    this->clear();
}

int 
Buffer::read_int8(int8_t &val)
{
    if (start_read_pos_ )
}
int Buffer::read_int16(int16_t &val);
int Buffer::read_int32(int32_t &val);
int Buffer::read_int64(int64_t &val);
int Buffer::read_string(string &str);
int Buffer::read_bytes(void *buf, int buf_size);

int Buffer::write_int8(int8_t val);
int Buffer::write_int16(int16_t val);
int Buffer::write_int32(int32_t val);
int Buffer::write_int64(int64_t val);
int Buffer::write_string(string str);
int Buffer::write_bytes(const void *buf, int buf_size);