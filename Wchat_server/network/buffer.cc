#include "buffer.h"

Buffer::Buffer()
{
    write_buffer_.reserve(MAX_BUFFER_SIZE);
    read_buffer_.reserve(MAX_BUFFER_SIZE);
    this->clear();
}
Buffer::~Buffer()
{
    this->clear();
}

int 
Buffer::read_int8(int8_t &val)
{
    if (this->empty()) {
        return 0;
    }

    val = read_buffer_[start_read_pos_];
    this->next_read_start_pos();

    return sizeof(int8_t);
}

int Buffer::read_int16(int16_t &val)
{
    if (this->size() < sizeof(int16_t)) {
        return 0;
    }

    char *ptr = read_buffer_.data;
    memcpy(&val, ptr+start_read_pos_, sizeof(int16_t));
    this->next_read_start_pos(sizeof(int16_t));

    return sizeof(int16_t);
}

int Buffer::read_int32(int32_t &val)
{
    if (this->size() < sizeof(int32_t)) {
        return 0;
    }

    char *ptr = read_buffer_.data;
    memcpy(&val, ptr+start_read_pos_, sizeof(int32_t));
    this->next_read_start_pos(sizeof(int32_t));

    return sizeof(int32_t);
}
int Buffer::read_int64(int64_t &val)
{
    if (this->size() < sizeof(int64_t)) {
        return 0;
    }

    char *ptr = read_buffer_.data;
    memcpy(&val, ptr+start_read_pos_, sizeof(int64_t));
    this->next_read_start_pos(sizeof(int64_t));

    return sizeof(int64_t);
}

// 字符串是以 ‘\0’ 结尾的
int Buffer::read_string(string &str)
{
    if (this->empty()) {
        return 0;
    }

    int str_size = 0;
    for (int i = start_read_pos_ ; i < end_read_pos_; ++i) {
        if (read_buffer_[i] == '\0') {
            str_size = i - start_read_pos_;
            break;
        }
        if (i - start_read_pos_ == MAX_STRING_SIZE) {
            str_size = MAX_STRING_SIZE;
            break;
        }
    }

    char *ptr = read_buffer_.data;
    char buf[MAX_STRING_SIZE] = {0};
    memcpy(buf, ptr + start_read_pos_, str_size);
    this->next_read_start_pos(str_size);
    str = buf;

    return str_size;
}
int Buffer::read_bytes(void *buf, int buf_size, bool match = false)
{
    if (this->empty() || buf == NULL) {
        return 0;
    }

    if (this->size() < buf_size ) { 
        if (match == false) {
            buf_size = this->size();
        } else {
            return 0;
        }
    }

    char *ptr = read_buffer_.data;
    memcpy(buf, ptr + start_read_pos_, buf_size);
    this->next_read_start_pos(buf_size);

    return buf_size;
}

int Buffer::write_int8(int8_t val);
int Buffer::write_int16(int16_t val);
int Buffer::write_int32(int32_t val);
int Buffer::write_int64(int64_t val);
int Buffer::write_string(string str);
int Buffer::write_bytes(const void *buf, int buf_size);