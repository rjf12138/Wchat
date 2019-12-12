#include "data_center.h"

DataCenter& 
DataCenter::get_datacenter_instance(void)
{
    static DataCenter data_center_instance;
    return data_center_instance;
}

int 
DataCenter::run_handler(void)
{
    while (!exit_) {
        if (msg_queue_.empty()) {
            usleep(200);
            continue;
        }

        InnerMsg ret;
        msg_queue_.pop(ret);
        for (auto iter = obj_map_.begin(); iter != obj_map_.end(); ++iter) {
            if (iter->first == ret.handler_) {
                iter->second.on_msg(ret);
            }
        }
    }
}

int 
DataCenter::exit_handler(void)
{
    exit_ = true;
}