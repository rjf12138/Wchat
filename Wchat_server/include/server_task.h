#ifndef __TASK_HANDLE__
#define __TASK_HANDLE__

#include <iostream>
#include <string>

#include "user_manager.h"
#include "group_manager.h"
#include "account_manager.h"
#include "connect_manager.h"


class ServerTask {
public:
    static ServerTask& get_server_task_instance(UserManager& user_manager_instance, GroupManager& group_manager_instance, \
                                                    AccountManager& account_manager_instance, ConnectManager& connect_manager_instance);
    int processing_tasks(void);
    int send_user_info(const std::string& user_account);
    int send_user_friend_info(const std::string& user_account);
    int send_user_managered_group_info(const std::string& user_account);
    int send_user_joined_group_info(const std::string& user_account);
    int send_group_info(const std::string& user_account, const std::string& group_account);

    int server_send_msg_to_user(const std::string& user_account, const std::string& data);

    int add_a_new_user_to_the_server(struct fragment& frag);
    int add_a_new_group_to_the_server(const std::string& group_name, const std::string& manager_account);
    int add_a_pair_of_friends_to_the_server(const std::string& friend_1_account, const std::string& friend_2_account);
    int add_member_to_the_group(const std::string& group_account, const std::string& user_account);

    int remove_a_user_from_server(const std::string& user_account);
    int remove_a_group_from_server(const std::string& group_account);
    int remove_a_pair_of_friends_from_the_server(const std::string& friend_1_account, const std::string& friend_2_account);
    int remove_member_from_the_group(const std::string& group_account, const std::string& user_account);

private:
    ServerTask(UserManager& user_manager, GroupManager& group_manager, AccountManager& account_manager, ConnectManager& connect_manager_instance);
    ~ServerTask() = default;
private:
    UserManager& user_manager;
    GroupManager& group_manager;
    AccountManager& account_manager;
    ConnectManager& d_connect_manager;
};

#endif