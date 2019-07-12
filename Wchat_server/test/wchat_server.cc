#include <iostream>
#include <string>

#include "msg_transmission.h"
#include "connect_manager.h"
#include "queue.h"
#include "msg_buffer.h"
#include "server_signal.h"
#include "thread_pool.h"

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

    ThreadPool handle_msgs;

    FRAGMENT_SEND_RECV_QUEUE_INIT(json_instance);
    MSG_BUFFER_INIT();

    Signal::signal(SIGALRM, [&connect_manager_instance]{connect_manager_instance.set_signal_trigger(true);});   // 捕获 alarm 发的信号
    connect_manager_instance.set_check_connect_alarm();                                                                                         // 设置闹钟定时检查链接寿命
    
    struct thread_arg process = {msg_transmission_instance, server_task, msg_handle, connect_manager_instance};

    handle_msgs.start_thread(process);
    handle_msgs.join_thread();

    LOG_INFO("Wchat exited!");

    return 0;
}