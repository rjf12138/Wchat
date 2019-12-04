#include "thread.h"

Thread::Thread(void) { }
Thread::~Thread(void) { }

void* 
Thread::create_func(void* arg)
{
    if (arg == NULL) {
        return NULL;
    }

    Thread *self = (Thread*)arg;
    pthread_cleanup_push(cleanup_func, arg);
    self->run_handler();
    pthread_cleanup_pop(1); // 参数设为为表示不被调用
}

void
Thread::cleanup_func(void *arg)
{
    if (arg == NULL) {
        return;
    }

    Thread *self = (Thread*)arg;
    self->exit_handler();
}

int 
Thread::init(void)
{
    return pthread_create(&thread_id_, NULL, create_func, (void*)this);
}

int Thread::run_handler(void) { }
int Thread::exit_handler(void) { }

int Thread::wait_thread(void)
{
    return pthread_join(thread_id_, NULL);
}

int Thread::stop(void)
{
    pthread_exit(NULL);
}
