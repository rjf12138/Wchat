#ifndef __SEND_MSG_H__
#define __SEND_MSG_H__

#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "server_json.h"
#include "server_log.h"
#include "msg_fragment.h"
#include "queue.h"

class SendMSG {
public:
    SendMSG* get_send_fragment_instance(void);
    void send_fragment_looping(void);
private:
    SendMSG();
    struct sockaddr get_client_sockaddr(const struct fragment &frag);
    int send_fragment_with_TCP(struct fragment &frag, );
private:
    int server_send_fd;
};

static SendMsg *send_fragment_instance = nullptr;

#endif