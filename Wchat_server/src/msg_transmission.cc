#include <cstdlib>
#include <cerrno>
#include <cstring>

#include <string>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "msg_transmission.h"

MsgTransmission* MsgTransmission::msg_transmission_self;

MsgTransmission::MsgTransmission(Json& json_instance, ConnectManager& connect_manager_instance)
                    :d_connect_manager_instance(connect_manager_instance)
{
    msg_transmission_self = this;   // 用于静态函数访问实例类的属性

    std::string addr;
    std::cout << __LINE__ << " " << SERVER_ADDR << std::endl;
    json_instance.read_value(SERVER_ADDR, addr);
    json_instance.read_value(SERVER_PORT, port);
    std::cout << "ser: " << addr << " port: " << port << std::endl;

    msg_transmission_self->server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (msg_transmission_self->server_fd == -1) {
        LOG_ERROR("MsgTransmission:: socket error: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    memset(&msg_transmission_self->serv_addr, '\0', sizeof(msg_transmission_self->serv_addr));
    msg_transmission_self->serv_addr.sin_family = AF_INET;
    msg_transmission_self->serv_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, addr.c_str(), (void*)&msg_transmission_self->serv_addr.sin_addr) == -1) {
        LOG_ERROR("MsgTransmission:: inet_pton error: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    socklen_t size = sizeof(msg_transmission_self->serv_addr);
    if (bind(msg_transmission_self->server_fd, (sockaddr *)&msg_transmission_self->serv_addr, size) == -1) {
        LOG_ERROR("MsgTransmission:: bind error: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (listen(msg_transmission_self->server_fd, 5) == -1) {
        LOG_ERROR("MsgTransmission:: listen error: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (evutil_make_socket_nonblocking(msg_transmission_self->server_fd) == -1) {
        LOG_ERROR("MsgTransmission:: evutil_make_socket_nonblocking error");
        exit(EXIT_FAILURE);
    }

    if ((msg_transmission_self->server_base = event_base_new()) == NULL) {
        LOG_ERROR("MsgTransmission:: event_base_new error");
        exit(EXIT_FAILURE);
    }

    if ((msg_transmission_self->server_event = event_new(msg_transmission_self->server_base, msg_transmission_self->server_fd, EV_READ | EV_PERSIST,do_accept, msg_transmission_self->server_base)) == NULL) {
        LOG_ERROR("MsgTransmission:: event_new error");
        exit(EXIT_FAILURE);
    }

    if (event_add(msg_transmission_self->server_event, NULL) == -1) {
        LOG_ERROR("MsgTransmission:: event_add error");
        exit(EXIT_FAILURE);
    }

}

MsgTransmission&
MsgTransmission::get_msg_transmission_instance(Json& json_instance, ConnectManager& connect_manager_instance)
{
    static MsgTransmission receive_fragment_instance(json_instance, connect_manager_instance);
    return receive_fragment_instance;
}

void
MsgTransmission::msg_transmission_looping(void)
{
    if (event_base_dispatch(msg_transmission_self->server_base) == -1) {
        LOG_ERROR("MsgTransmission:: event_base_dispatch error");
        exit(EXIT_FAILURE);
    }
}

void
MsgTransmission::exit_transmission(void)
{
    struct timeval exit_time = {5, 0};
    event_base_loopexit(msg_transmission_self->server_base, &exit_time);
    LOG_INFO("MsgTransmission::exit_transmission called.");
}

void 
MsgTransmission::do_accept(int sockfd, short event_type, void *arg)
{
    struct event_base *tmp_base = (struct event_base*)arg;
    evutil_socket_t fd;
    struct sockaddr_in cliaddr;

    socklen_t size = sizeof(cliaddr);
    if ((fd = accept(sockfd, (sockaddr*)&cliaddr, &size)) == -1) {
        LOG_ERROR("MsgTransmission:: accept error: %s", strerror(errno));
        return ;
    }
    
    struct bufferevent* tmp_bufferevent;
    if ((tmp_bufferevent = bufferevent_socket_new(msg_transmission_self->server_base, fd, BEV_OPT_CLOSE_ON_FREE)) == NULL) {
        LOG_ERROR("MsgTransmission:: bufferevent_socket_new error");
        return ;
    }

    bufferevent_setcb(tmp_bufferevent, read_cb, write_cb, error_cb, (void*)arg);
    bufferevent_enable(tmp_bufferevent, EV_READ | EV_WRITE | EV_PERSIST);
}

void 
MsgTransmission::read_cb(struct bufferevent *buf, void *arg)
{
    struct fragment tmp_fragment;

    if (bufferevent_read(buf, (void*)&tmp_fragment, sizeof(tmp_fragment)) == -1) {
        LOG_ERROR("MsgTransmission:: bufferevent error");
        exit(EXIT_FAILURE);
    } else {
        if (msg_transmission_self->d_connect_manager_instance.check_msg_is_send_from_unknown_user(tmp_fragment) == true) {
            msg_transmission_self->d_connect_manager_instance.add_unknown_connect(tmp_fragment, buf);
        }
        FRAGMENT_RECV_PUSH(tmp_fragment);
    }
}

void
MsgTransmission::write_cb(struct bufferevent *buf, void *arg)
{

}

void 
MsgTransmission::error_cb(struct bufferevent *buf, short event, void *arg)
{
    /*
    *
    * TODO： 加上客户端信息打印
    * 
    */
    if (event & BEV_EVENT_TIMEOUT) {
        LOG_ERROR("time out");
    } else if (event & BEV_EVENT_EOF) {
        LOG_INFO("connect closed");
    } else if (event & BEV_EVENT_ERROR) {
        LOG_ERROR("other error");
    }
}

void 
MsgTransmission::send_msg_to_client(void)
{
    struct fragment frag;
    struct bufferevent* buf;

    if (FRAGMENT_SEND_SIZE() > 0) {
        FRAGMENT_SEND_POP(frag);
        if (frag.msg_type == SIGN_UP_SUCCESSED) {
            buf = msg_transmission_self->d_connect_manager_instance.get_unknown_connect(frag.tmp);
        } else {
            buf = msg_transmission_self->d_connect_manager_instance.get_user_connect(std::string(frag.receiver_account));
        }
        bufferevent_write(buf, (void*)&frag, sizeof(frag));
    }
}

MsgTransmission::~MsgTransmission()
{
    event_free(msg_transmission_self->server_event);
    event_base_free(msg_transmission_self->server_base);
}