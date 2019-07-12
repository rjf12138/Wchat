#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

#include "server_signal.h"
#include "msg_handler.h"
#include "server_task.h"
#include "server_log.h"
#include "msg_transmission.h"

#define THREAD_NUM          5

struct thread_arg {
    MsgTransmission& msg_transmission_instance;
    ServerTask& server_task_instance;
    MsgHandler& msg_handler_instance;
    ConnectManager& connect_manager_instance;
};

struct ThreadFunc {
    static void* msg_handler_thread(void* args);
    static void* server_task_thread(void* args);
    static void* send_msg_thread(void* args);
    static void* connect_manager_thread(void* args);
    static void* msg_transmission_looping_thread(void* args);
};

class ThreadPool {
public:
    void start_thread(struct thread_arg& args);
    void join_thread(void);
private:
    pthread_t thread_id[THREAD_NUM];
};

#endif