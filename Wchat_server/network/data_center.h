#ifndef __DATACENTER_H__
#define __DATACENTER_H__

#include "basic_head.h"
#include "inner_msg_object.h"
#include "thread.h"
#include "queue.h"

class DataCenter : public Thread{
public:
    static DataCenter& get_datacenter_instance(void);

    virtual int run_handler(void) override;
    virtual int exit_handler(void) override;

    static void TransMsg(OBJ_HANDLE hender, InnerMsg msg);
private:
    DataCenter(void);
    ~DataCenter(void);

private:
    static map<OBJ_HANDLE, MSGObject> obj_map_;
    Queue<InnerMsg> msg_queue_;
    bool exit_ = false;
};

#define DATACENTER_INSTANCE DataCenter::get_datacenter_instance()

#endif