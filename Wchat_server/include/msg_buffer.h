#ifndef __MSG_BUFFER_H__
#define __MSG_BUFFER_H__

#include <map>
#include <vector>
#include <string>
#include <memory>

#include <unistd.h>
#include <pthread.h>

#include "queue.h"
#include "server_log.h"
#include "msg_fragment.h"

class MsgBuffer {
public:
    MsgBuffer();
    int push(const struct fragment &msg);
    int pop(const std::string& account, std::vector<struct fragment>& msgs);

    ~MsgBuffer();
private:
    std::multimap<std::string, struct fragment> msg_buffer;
    pthread_mutex_t mutex;
};

std::shared_ptr<MsgBuffer> msg_ptr;

#define MSG_BUFFER_INIT()                   do {msg_ptr = std::make_shared<MsgBuffer>();} while(false)
#define MSG_BUFFER_PUSH(param)              msg_ptr->push(param)
#define MSG_BUFFER_POP(param1, param2)      msg_ptr->pop(param1, param2)

#endif