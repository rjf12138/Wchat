#include "thread_pool.h"

void*
ThreadFunc::msg_handler_thread(void* args)
{
    struct thread_arg* msg_handler = (struct thread_arg*)args;
    
    while (!Exit::stop()) {
        msg_handler->msg_handler_instance.handle_message();
    }
    LOG_INFO("ThreadFunc::msg_handler_thread() exit.");
}

void*
ThreadFunc::server_task_thread(void* args)
{
    struct thread_arg* server_task = (struct thread_arg*)args;

    while (!Exit::stop()) {
        server_task->server_task_instance.processing_tasks();
    }
    LOG_INFO("ThreadFunc::server_task_thread() exit.");
}

void*
ThreadFunc::send_msg_thread(void* args) 
{
    struct thread_arg* send_msg_instance = (struct thread_arg*)args;

    while (!Exit::stop()) {
        send_msg_instance->msg_transmission_instance.send_msg_to_client();
    }
    LOG_INFO("ThreadFunc::send_msg_thread() exit.");
}

void*
ThreadFunc::msg_transmission_looping_thread(void* args)
{
    struct thread_arg* event_looping = (struct thread_arg*)args;

    event_looping->msg_transmission_instance.msg_transmission_looping();

    LOG_INFO("ThreadFunc::msg_transmission_looping_thread() exit.");
}

void*
ThreadFunc::connect_manager_thread(void* args)
{
    struct thread_arg* t_connect_manager = (struct thread_arg*)args;

    while (!Exit::stop()) {
        if (t_connect_manager->connect_manager_instance.is_signal_trigger()) {
            t_connect_manager->connect_manager_instance.remove_timeout_user_connect();
            t_connect_manager->connect_manager_instance.remove_timeout_unknown_connect();
            t_connect_manager->connect_manager_instance.set_signal_trigger(false);
            t_connect_manager->connect_manager_instance.set_check_connect_alarm();
        }
    }
}

void
ThreadPool::start_thread(struct thread_arg& arg)
{
    void* (*thread_func[THREAD_NUM])(void*) = {
        ThreadFunc::msg_handler_thread, 
        ThreadFunc::server_task_thread, 
        ThreadFunc::send_msg_thread, 
        ThreadFunc::msg_transmission_looping_thread, 
        ThreadFunc::connect_manager_thread
    };

    for (int i = 0; i < THREAD_NUM; ++i) {
        int th_errno = pthread_create(&thread_id[i], NULL, thread_func[i], (void*)&arg);
        if (th_errno != 0) {
            LOG_ERROR("ThreadPool::start_thread() failed: pthread_create() calls failed: %s", strerror(th_errno));
            exit(EXIT_FAILURE);
        }
    }
}

void 
ThreadPool::join_thread(void)
{
    for (int i = 0; i < THREAD_NUM; ++i) {
        pthread_join(thread_id[i], NULL);
    }
}