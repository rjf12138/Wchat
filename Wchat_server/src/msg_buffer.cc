#include "msg_buffer.h"

MsgBuffer::MsgBuffer()
{
    pthread_mutex_init(&mutex, NULL);
}

MsgBuffer::~MsgBuffer()
{
    pthread_mutex_destroy(&mutex);
}

int
MsgBuffer::push(const struct fragment &msg)
{
    pthread_mutex_lock(&mutex);
    msg_buffer.insert(std::pair<std::string, struct fragment>(msg.receiver_account, msg));
    pthread_mutex_unlock(&mutex);
    return 0;
}

int
MsgBuffer::pop(const std::string& account, std::vector<struct fragment>& msgs)
{
    pthread_mutex_lock(&mutex);
    auto result = msg_buffer.equal_range(account);
    msgs.clear();
    for (auto tmp_iter = result.first; tmp_iter != result.second; ++tmp_iter) {
        msgs.push_back(tmp_iter->second);
    }
    msg_buffer.erase(account);
    pthread_mutex_unlock(&mutex);
    return 0;
}