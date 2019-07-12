#include <iostream>
#include <string>

#include "msg_transmission.h"
#include "queue.h"
#include "server_signal.h"

int main(void)
{
    Exit::init_exit();

    Json& json_instance = Json::get_json_instance("../../server_config/server.json");
    SQLiteWrapper& sqlite_instance = SQLiteWrapper::get_sqlitewrapper_instance(json_instance);
    AccountManager& account_manager_instance = AccountManager::get_account_manager_instance(json_instance);
    UserManager& user_manager_instance = UserManager::get_user_manager_instance(sqlite_instance, json_instance);
    GroupManager& group_manager_instance = GroupManager::get_group_manager_instance(sqlite_instance);

    ConnectManager& connect_manager_instance = ConnectManager::get_connect_manager_instance();
    MsgHandler& msg_handle = MsgHandler::get_msg_handler_instance(user_manager_instance, group_manager_instance);
    ServerTask& server_task = ServerTask::get_server_task_instance(user_manager_instance, group_manager_instance, account_manager_instance, connect_manager_instance);
    MsgTransmission& msg_transmission_instance = MsgTransmission::get_msg_transmission_instance(json_instance, connect_manager_instance);

    FRAGMENT_SEND_RECV_QUEUE_INIT(json_instance);

    msg_transmission_instance.msg_transmission_looping();

    std::cout << "program done!" << std::endl;
    return 0;
}