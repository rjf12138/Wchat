#include <unistd.h>
#include <pthread.h>

#include "account_manager.h"
#include "server_json.h"
#include "server_log.h"
#include "queue.h"
#include "msg_buffer.h"
#include "msg_type.h"
#include "sqlite_wrapper.h"
#include "user_manager.h"
#include "msg_fragment.h"
#include "msg_handler.h"
#include "server_task.h"
#include "connect_manager.h"

struct arg {
    Json& json_instance;
    SQLiteWrapper& sqlite_instance;
    AccountManager& account_manager_instance;
    UserManager& user_manager_instance;
    GroupManager& group_manger_instance;
    MsgHandler& msg_handle;
    ServerTask& server_task;
};

bool exit_status = false;

void sig_func(int sig)
{
    exit_status = true;
}

void print_frag(const struct fragment& frag)
{
    std::cout << "msg_type: " << (int)frag.msg_type << std::endl;
    std::cout << "sender_account: " << frag.sender_account << std::endl;
    std::cout << "receiver_account: " <<  frag.receiver_account << std::endl;
    std::cout << "group_account: " << frag.group_account << std::endl;
    std::cout << "fragment_amount: " << (int)frag.fragment_amount << std::endl;
    std::cout << "fragment_position: " << (int)frag.fragment_position << std::endl;
    std::cout << "fragment_length: " << (int)frag.fragment_length << std::endl;
    std::cout << "fragment_identity: " << frag.identity << std::endl;
    std::cout << "fragment_data: " << frag.data << std::endl;
}

void* msg_handle_function(void* args)
{
    struct arg *param = (struct arg*)args;
    std::cout << "start msg_handle_function" << std::endl;

    while (!exit_status) {
        param->msg_handle.handle_message();
    }
}

void* server_task_function(void* args)
{
    struct arg *param = (struct arg*)args;
    std::cout << "start server_task_function" << std::endl;

    while (!exit_status) {
        param->server_task.processing_tasks();
    }
}

void* received_data_function(void* args)
{
    struct fragment frag;
    std::string tmp_str;

    std::cout << "start received_data_function" << std::endl;
    std::fstream frag_file("../../server_config/tmp_fragment");
    if (!frag_file) {
        std::cerr << "open ../../server_config/tmp_fragment failed." << std::endl;
        return NULL;
    }
    frag_file >> tmp_str;
    frag.msg_type = std::stoi(tmp_str); 
    frag_file >> tmp_str;
    sprintf(frag.sender_account, "%s", tmp_str.c_str());
    frag_file >> tmp_str;
    sprintf(frag.receiver_account, "%s", tmp_str.c_str());
    frag_file >> tmp_str;
    sprintf(frag.group_account, "%s", tmp_str.c_str());
    frag_file >> frag.fragment_amount;
    frag.fragment_amount -= '0'; 
    frag_file >> frag.fragment_position;
    frag.fragment_position -= '0';
    frag_file >> tmp_str;
    for (int i = 0; i < tmp_str.size(); ++i) {
        if (tmp_str[i] == '+') {
            tmp_str[i] = ' ';
            break;
        }
    }
    sprintf(frag.data, "%s", tmp_str.c_str());
    frag.fragment_length = tmp_str.size();
    std::cout << "str_size: " << tmp_str.size() << std::endl;
    std::cout << "frag_size: " << (int)frag.fragment_length << std::endl;
    print_frag(frag);
    FRAGMENT_RECV_PUSH(frag);
}

void* send_data_function(void* args)
{
    struct fragment frag;
    std::cout << "start send_data_function" << std::endl;

    while (!exit_status) {
        if (FRAGMENT_SEND_SIZE() > 0) {
            std::cout << "send_data_function(): pop a data." << std::endl;
            FRAGMENT_SEND_POP(frag);
            print_frag(frag);
        }
    }
}

int main(void)
{
    if (signal(SIGINT, sig_func) == SIG_ERR) {
        perror("signal");
        exit(EXIT_FAILURE);
    }


    Json& json_instance = Json::get_json_instance("../../server_config/server.json");
    SQLiteWrapper& sqlite_instance = SQLiteWrapper::get_sqlitewrapper_instance(json_instance);
    AccountManager& account_manager_instance = AccountManager::get_account_manager_instance(json_instance);
    UserManager& user_manager_instance = UserManager::get_user_manager_instance(sqlite_instance, json_instance);
    GroupManager& group_manager_instance = GroupManager::get_group_manager_instance(sqlite_instance);

    ConnectManager& connect_manager_instance = ConnectManager::get_connect_manager_instance();
    MsgHandler& msg_handle = MsgHandler::get_msg_handler_instance(user_manager_instance, group_manager_instance);
    ServerTask& server_task = ServerTask::get_server_task_instance(user_manager_instance, group_manager_instance, account_manager_instance, connect_manager_instance);

    FRAGMENT_SEND_RECV_QUEUE_INIT(json_instance);
    MSG_BUFFER_INIT();

    pthread_t thread[4];
    void* (*thread_func[4])(void*) = {msg_handle_function, server_task_function,received_data_function, send_data_function};
    struct arg thread_arg = {
                                json_instance, 
                                sqlite_instance, 
                                account_manager_instance, 
                                user_manager_instance,
                                group_manager_instance,
                                msg_handle,
                                server_task
                            };

    int err;
    for (int i = 0; i < 4; ++i) {
        err = ::pthread_create(&thread[i], NULL, thread_func[i], (void*)&thread_arg);
        if (err != 0) {
            std::cerr << "pthread_create line: " << i << " " << strerror(err) << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    std::cout << "stop!" << std::endl;
    for (int i = 0; i < 4; ++i) {
        err = ::pthread_join(thread[i], NULL);
        if (err != 0) {
            std::cerr << "pthread_join line: " << i << " " << strerror(err) << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    std::cout << __FILE__ << " OVER!" << std::endl;
    return 0;
}