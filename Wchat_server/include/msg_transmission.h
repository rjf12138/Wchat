#ifndef __SEND__RECV_H__
#define __SEND__RECV_H__

#include <unistd.h>
#include <event2/event.h>
#include <event2/bufferevent.h>

#include "msg_fragment.h"
#include "queue.h"
#include "server_json.h"
#include "server_log.h"
#include "server_task.h"
#include "server_signal.h"
#include "msg_handler.h"
#include "connect_manager.h"

#define SERVER_ADDR "server-addr"
#define SERVER_PORT "server-port"

class MsgTransmission {
public:
    static MsgTransmission& get_msg_transmission_instance(Json& json_instance, ConnectManager& connect_manager_instance);
    static void send_msg_to_client(void);
    static void msg_transmission_looping(void);

    static MsgTransmission* msg_transmission_self;
    static void exit_transmission(void);

    ~MsgTransmission();
private:
    MsgTransmission();
    MsgTransmission(Json& json_instance, ConnectManager& connect_manager_instance);

    static void do_accept(int sockfd, short event_type, void *arg);
    static void read_cb(struct bufferevent *buf, void *arg);
    static void write_cb(struct bufferevent *buf, void *arg);
    static void error_cb(struct bufferevent *buf, short error, void *arg);
    static void exit_cb(int exit_fd, short what, void *arg);
private:
    int server_fd;
    int exit_fd;
    short port;
    struct sockaddr_in serv_addr;
    int i = 0;

    struct event *server_event;
    struct event_base *server_base;

    ConnectManager& d_connect_manager_instance;
};


#endif