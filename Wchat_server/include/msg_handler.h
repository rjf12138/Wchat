#ifndef __MSG_HANDLE_H__
#define __MSG_HANDLE_H__

#include <iostream>
#include <string>

#include "user_manager.h"
#include "group_manager.h"

class MsgHandler {
public:
    static MsgHandler& get_msg_handler_instance(UserManager& user_manager_instance, GroupManager& group_manager_instance);
    int handle_message(void);

private:
    MsgHandler(UserManager& user_manager_instance, GroupManager& group_manager_instance);
    ~MsgHandler() = default;
private:
    UserManager& user_manager;
    GroupManager& group_manager;
};

#endif