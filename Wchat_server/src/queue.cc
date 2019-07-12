#include "queue.h"

Queue::Queue(Json& json_instance)
{
    json_instance.read_value(MAX_QUEUE_SIZE, max_queue_size);
    current_size = 0;
    pthread_mutex_init(&queue_mutex,NULL);
}

Queue::~Queue()
{
    pthread_mutex_destroy(&queue_mutex);
}

int
Queue::push(const struct fragment &frag)
{
    pthread_mutex_lock(&queue_mutex);
    if(current_size == max_queue_size){
        pthread_mutex_unlock(&queue_mutex);
        return -1;
    }

    frag_queue.push(frag);
    current_size++;
    pthread_mutex_unlock(&queue_mutex);
    return 0;
}

int
Queue::pop(struct fragment& frag)
{
    pthread_mutex_lock(&queue_mutex);
    if(current_size == 0){
        pthread_mutex_unlock(&queue_mutex);
        return -1;
    }
    frag = frag_queue.front();
    frag_queue.pop();
    current_size--;
    pthread_mutex_unlock(&queue_mutex);
    return 0;
}