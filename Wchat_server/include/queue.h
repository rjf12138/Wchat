#ifndef __SEND_RECV_QUEUE_H__
#define __SEND_RECV_QUEUE_H__

#include "msg_fragment.h"
#include "server_json.h"

#include <vector>
#include <queue>

#include <unistd.h>
#include <pthread.h>

#define MAX_QUEUE_SIZE  "server-queue-max-size"

class Queue{
public:
    Queue(Json& json_instance);
    int push(const struct fragment &frag);
    int pop(struct fragment &frag);
    int size(void) {return current_size;}
    ~Queue();
private:
    std::queue<struct fragment>     frag_queue;
    pthread_mutex_t queue_mutex;
    int max_queue_size;
    int current_size;
};

std::shared_ptr<Queue>  fragment_send_queue_ptr;
std::shared_ptr<Queue>  fragment_recv_queue_ptr;
std::shared_ptr<Queue>  fragment_inner_transmisson_ptr; 

#define     FRAGMENT_SEND_RECV_QUEUE_INIT(frag)                                         \
                do {                                                                    \
                        fragment_send_queue_ptr = std::make_shared<Queue>(frag);        \
                        fragment_recv_queue_ptr = std::make_shared<Queue>(frag);        \
                        fragment_inner_transmisson_ptr = std::make_shared<Queue>(frag); \
                } while(false)

#define     FRAGMENT_SEND_POP(frag)             fragment_send_queue_ptr->pop(frag)
#define     FRAGMENT_SEND_PUSH(frag)            fragment_send_queue_ptr->push(frag)
#define     FRAGMENT_SEND_SIZE()                fragment_send_queue_ptr->size()

#define     FRAGMENT_RECV_POP(frag)             fragment_recv_queue_ptr->pop(frag)
#define     FRAGMENT_RECV_PUSH(frag)            fragment_recv_queue_ptr->push(frag)
#define     FRAGMENT_RECV_SIZE()                fragment_recv_queue_ptr->size()

#define     FRAGMENT_INNER_TRANSMISSON_POP(frag)    fragment_inner_transmisson_ptr->pop(frag)
#define     FRAGMENT_INNER_TRANSMISSON_PUSH(frag)    fragment_inner_transmisson_ptr->push(frag)
#define     FRAGMENT_INNER_TRANSMISSON_SIZE()        fragment_inner_transmisson_ptr->size()

#endif