#include <vector>

#include "queue.h"
#include "msg_buffer.h"
#include "msg_fragment.h"
#include "fragment_operate.h"
#include "user_manager.h"
#include "group_manager.h"
#include "sqlite_wrapper.h"
#include "server_log.h"
#include "server_task.h"
#include "msg_type.h"
#include "account_manager.h"

ServerTask&
ServerTask::get_server_task_instance(UserManager& user_manager_instance, GroupManager& group_manager_instance, 
                                        AccountManager& account_manager_instance, ConnectManager& connect_manager_instance)
{
    static ServerTask server_task_instance(user_manager_instance, group_manager_instance, account_manager_instance, connect_manager_instance);
    return server_task_instance;
}

ServerTask::ServerTask(UserManager& user_manager_instance, GroupManager& group_manager_instance, AccountManager& account_manager_instance, ConnectManager& connect_manager_instance)
            : user_manager(user_manager_instance), group_manager(group_manager_instance), account_manager(account_manager_instance), d_connect_manager(connect_manager_instance)
{

} 

int
ServerTask::server_send_msg_to_user(const std::string& user_account, const std::string& data)
{
    struct fragment frag;
    FragmentOperate::set_msg_type(frag, SERVER_SEND_MSG_TO_USER);
    FragmentOperate::set_receiver_account(frag, user_account);
    FragmentOperate::set_and_push_data_to_queue(frag, data, user_manager.is_user_online(user_account));
}

int
ServerTask::send_user_info(const std::string& user_account)
{
    struct user tmp_user;
    if (user_manager.get_user_info(user_account, tmp_user) == -1) {
        LOG_WARN("ServerTask:: send_user_info() failed: UserManager::get_user_info calls failed.");
        return -1;
    }

    std::string data = tmp_user.user_account + ' ' + tmp_user.user_name;

    struct fragment frag;
    FragmentOperate::set_msg_type(frag, SERVER_SEND_USER_INFO);
    FragmentOperate::set_receiver_account(frag, user_account);
    FragmentOperate::set_and_push_data_to_queue(frag, data, user_manager.is_user_online(user_account));

    return 0;
}

int
ServerTask::send_user_friend_info(const std::string& user_account)
{
    std::vector<std::string> friend_accounts;
    if (user_manager.get_all_user_friends_account(user_account, friend_accounts) == -1) {
        LOG_WARN("ServerTask:: send_user_friend_info() failed: UserManager::get_all_user_friends_account calls failed.");
        return -1;
    }

    std::vector<std::string> friend_names;
    if (user_manager.get_user_name_according_to_the_user_account(friend_accounts, friend_names) == -1) {
        LOG_WARN("ServerTask:: send_user_friend_info() failed: UserManager::get_user_name_according_to_the_user_account calls failed.");
        return -1;
    }

    if (friend_accounts.size() != friend_names.size()) {
        LOG_WARN("ServerTask:: send_user_friend_info() failed: friends accounts amount is not equal to friends_name amount.");
        return -1;
    }

    std::string data;
    for (int i = 0; i < friend_accounts.size(); ++i) {
        data += friend_accounts[i] + ' ' + friend_names[i] + ' ';
    }

    struct fragment tmp_frag;

    FragmentOperate::set_msg_type(tmp_frag, SERVER_SEND_USER_FRIENDS_INFO);
    FragmentOperate::set_receiver_account(tmp_frag, user_account);
    FragmentOperate::set_and_push_data_to_queue(tmp_frag, data, user_manager.is_user_online(user_account));

    return 0;
}

int
ServerTask::send_user_joined_group_info(const std::string& user_account)
{
    std::vector<std::string> group_accounts;
    if (user_manager.get_all_user_joined_groups_account(user_account, group_accounts) ==  -1) {
        LOG_WARN("ServerTask:: send_user_joined_group_info() failed: UserManager::get_all_user_joined_groups_account calls failed.");
        return -1;
    }

    std::vector<std::string> group_names;
    if (group_manager.get_group_name_according_to_the_group_account(group_accounts, group_names) == -1) {
        LOG_WARN("ServerTask:: send_user_joined_group_info() failed: GroupManager::get_group_name_according_to_the_group_account calls failed.");
        return -1;
    }

    if (group_accounts.size() != group_names.size()) {
        LOG_WARN("ServerTask:: send_user_joined_group_info() failed: group accounts amount is not equal to group_names.");
        return -1;
    }

    std::string data;
    for (int i = 0; i < group_accounts.size(); ++i) {
        data += group_accounts[i] + ' ' + group_names[i] + ' ';
    }
    
    struct fragment tmp_frag;
    FragmentOperate::set_msg_type(tmp_frag, SERVER_SEND_USER_JOINED_GROUPS_INFO);
    FragmentOperate::set_receiver_account(tmp_frag, user_account);
    FragmentOperate::set_and_push_data_to_queue(tmp_frag, data, user_manager.is_user_online(user_account));

    return 0;
}

int
ServerTask::send_user_managered_group_info(const std::string& user_account)
{
    std::vector<std::string> group_accounts;
    if (user_manager.get_all_user_joined_groups_account(user_account, group_accounts) ==  -1) {
        LOG_WARN("ServerTask:: send_user_managered_group_info() failed: UserManager::get_all_user_joined_groups_account calls failed.");
        return -1;
    }

    std::vector<std::string> group_names;
    if (group_manager.get_group_name_according_to_the_group_account(group_accounts, group_names) == -1) {
        LOG_WARN("ServerTask:: send_user_managered_group_info() failed: GroupManager::get_group_name_according_to_the_group_account calls failed.");
        return -1;
    }

    if (group_accounts.size() != group_names.size()) {
        LOG_WARN("ServerTask:: send_user_managered_group_info() failed: group accounts amount is not equal to group_names.");
        return -1;
    }

    std::string data;
    for (int i = 0; i < group_accounts.size(); ++i) {
        data += group_accounts[i] + ' ' + group_names[i] + ' ';
    }
    
    struct fragment tmp_frag;
    FragmentOperate::set_msg_type(tmp_frag, SERVER_SEND_USER_MANAGER_GROUPS_INFO);
    FragmentOperate::set_receiver_account(tmp_frag, user_account);
    FragmentOperate::set_and_push_data_to_queue(tmp_frag, data, user_manager.is_user_online(user_account));

    return 0;
}

int 
ServerTask::send_group_info(const std::string& user_account, const std::string& group_account)
{
    struct group tmp_group;
    if (group_manager.get_group_info(group_account, tmp_group) == -1) {
        LOG_WARN("ServerTask::send_group_info() failed: GroupManager::get_group_info() calls failed.");
        return -1;
    }

    std::vector<std::string> member_names;
    if (user_manager.get_user_name_according_to_the_user_account(tmp_group.member_accounts, member_names) == -1) {
        LOG_WARN("ServerTask::send_group_info() failed: UserManager::get_user_name_according_to_the_user_account() calls failed.");
        return -1;
    }

    if (tmp_group.member_accounts.size() != member_names.size()) {
        LOG_WARN("ServerTask:: send_group_info() failed: member accounts amount is not equal to member_name amount.");
        return -1;
    }

    // data 格式： group_account group_name group_manager group_member_account1 group_member_name1 ...
    std::string data;
    data += tmp_group.group_account + ' ' + tmp_group.group_name + ' ' + tmp_group.group_manager_account + ' ';
    for (int i = 0; i < member_names.size(); ++i) {
        data += tmp_group.member_accounts[i] + ' ' + member_names[i] + ' ';
    }

    struct fragment tmp_frag;
    FragmentOperate::set_msg_type(tmp_frag, SERVER_SEND_GROUP_INFO);
    FragmentOperate::set_receiver_account(tmp_frag, user_account);
    FragmentOperate::set_and_push_data_to_queue(tmp_frag, data, user_manager.is_user_online(user_account));

    return 0;
}

int
ServerTask::add_a_new_user_to_the_server(struct fragment& frag)
{
    int i;
    for (i = 0; i < frag.fragment_length; ++i) {
        if (frag.data[i] == ' ') {
            break;
        }
    }
    struct user new_user;
    new_user.user_account = account_manager.next_user_account();
    // data field : user_name + ' ' + user_password
    new_user.user_name = std::string(frag.data, 0, i);
    new_user.user_password = std::string(frag.data, i+1, frag.fragment_length-i-1);

    if (user_manager.add_new_user(new_user) == -1) {
        LOG_WARN("ServerTask::add_a_new_user_to_the_server() failed: UserManager::add_new_user() calls failed.");
        return -1;
    }

    struct fragment tmp_frag;
    tmp_frag.tmp = frag.tmp;
    FragmentOperate::set_msg_type(tmp_frag, SIGN_UP_SUCCESSED);
    FragmentOperate::set_receiver_account(tmp_frag, new_user.user_account);
    FragmentOperate::push_fragment_to_queue(tmp_frag, true);    // 对于新用户为了发送注册成功的消息，需要将其在线情况设置为 true

    return 0;
}

int
ServerTask::add_a_new_group_to_the_server(const std::string& group_name, const std::string& manager_account)
{
    struct group new_group;
    new_group.group_account = account_manager.next_group_account();
    new_group.group_name = group_name;
    new_group.group_manager_account = manager_account;
    
    if (group_manager.add_new_group(new_group) == -1) {
        LOG_WARN("ServerTask::add_a_new_group_to_server() failed: GroupManager::add_new_group() calls failed.");
        return -1;
    }

    if (group_manager.add_member_to_group(new_group.group_account, manager_account) == -1) {
        LOG_WARN("ServerTask::add_a_new_group_to_server() failed: GroupManager::add_member_to_group() calls failed.");
        return -1;
    }
    
    if (user_manager.add_joined_group_to_user(manager_account, new_group.group_account) == -1) {
        LOG_WARN("ServerTask::add_a_new_group_to_server() failed: GroupManager::add_joined_group_to_user() calls failed.");
        return -1;
    }
    
    if (user_manager.add_managered_group_to_user(manager_account, new_group.group_account) == -1) {
        LOG_WARN("ServerTask::add_a_new_group_to_server() failed: GroupManager::add_managered_group_to_user() calls failed.");
        return -1;
    }

    struct fragment tmp_frag;
    FragmentOperate::set_msg_type(tmp_frag, CREATE_NEW_GROUP_SUCCESSED);
    FragmentOperate::set_receiver_account(tmp_frag, manager_account);
    FragmentOperate::push_fragment_to_queue(tmp_frag, user_manager.is_user_online(manager_account));

    return 0;
}

int
ServerTask::add_a_pair_of_friends_to_the_server(const std::string& friend_1_account, const std::string& friend_2_account)
{
    if (user_manager.add_friend_to_user(friend_1_account, friend_2_account) == -1) {
        LOG_WARN("ServerTask::add_a_pair_of_friends_to_the_server() failed: UserManager::add_friend_to_user() calls failed: %d.", __LINE__);
        return -1;
    }

    if (user_manager.add_friend_to_user(friend_2_account, friend_1_account) == -1) {
        user_manager.remove_friend_from_user(friend_1_account, friend_2_account);
        LOG_WARN("ServerTask::add_a_pair_of_friends_to_the_server() failed: UserManager::add_friend_to_user() calls failed: %d.", __LINE__);
        return -1;
    }

    struct fragment tmp_frag;
    FragmentOperate::set_msg_type(tmp_frag, SERVER_NOTIFY_USER_THE_RESULT_OF_BE_FRIEND_REQUEST_SUCCESSED);
    FragmentOperate::set_receiver_account(tmp_frag, friend_1_account);
    FragmentOperate::push_fragment_to_queue(tmp_frag, user_manager.is_user_online(friend_1_account));

    FragmentOperate::set_receiver_account(tmp_frag, friend_2_account);
    FragmentOperate::push_fragment_to_queue(tmp_frag, user_manager.is_user_online(friend_2_account));

    return 0;
}

int
ServerTask::add_member_to_the_group(const std::string& group_account, const std::string& member_account)
{
    if (group_manager.add_member_to_group(group_account, member_account) == -1) {
        LOG_WARN("ServerTask::add_member_to_the_group() failed: UserManager::add_member_to_group() calls failed.");
        return -1;
    }

    if (user_manager.add_joined_group_to_user(member_account, group_account) == -1) {
        group_manager.remove_member_from_group(group_account, member_account);
        LOG_WARN("ServerTask::add_member_to_the_group() failed: UserManager::add_member_to_group() calls failed.");
        return -1;
    }

    struct fragment tmp_frag;
    FragmentOperate::set_msg_type(tmp_frag, SERVER_NOTIFY_USER_THE_RESULT_OF_JOIN_GROUP_REQUEST_SUCCESSED);
    FragmentOperate::set_receiver_account(tmp_frag, member_account);
    FragmentOperate::push_fragment_to_queue(tmp_frag, user_manager.is_user_online(member_account));

    struct group tmp_group_info;
    if (group_manager.get_group_info(group_account, tmp_group_info) == -1) {
        LOG_WARN("ServerTask::add_member_to_the_group() failed: UserManager::get_group_info() calls failed.");
        return -1;
    }

    FragmentOperate::set_receiver_account(tmp_frag, tmp_group_info.group_manager_account);
    FragmentOperate::push_fragment_to_queue(tmp_frag, user_manager.is_user_online(tmp_group_info.group_manager_account));

    return 0;
}

int
ServerTask::remove_a_user_from_server(const std::string& user_account)
{
    // TODO
    LOG_WARN("ServerTask::remove_a_user_from_server() not implement --- TODO.");
    return 0;
}

int
ServerTask::remove_a_group_from_server(const std::string& group_account)
{
    struct group tmp_group_info;
    if (group_manager.get_group_info(group_account, tmp_group_info) == -1) {
        LOG_WARN("ServerTask::remove_a_group_from_server failed: GroupManager::get_group_info() calls failed.");
        return -1;
    }

    for (const auto& member_account : tmp_group_info.member_accounts) {
        if (user_manager.remove_joined_group_from_user(member_account, group_account) == -1) {
            LOG_WARN("ServerTask::remove_a_group_from_server(): remove group %s from user %s joined table failed.", group_account.c_str(), member_account.c_str());
        }
    }
    user_manager.remove_managered_group_from_user(tmp_group_info.group_manager_account, group_account);

    struct fragment tmp_frag;
    FragmentOperate::set_msg_type(tmp_frag, USER_QUIT_FROM_GROUP);

    for (const auto& member_account : tmp_group_info.member_accounts) {
        if (member_account.compare(tmp_group_info.group_manager_account) == 0) {
            FragmentOperate::set_msg_type(tmp_frag, REMOVE_A_GROUP_SUCCESSED);
            FragmentOperate::set_receiver_account(tmp_frag, member_account);
            FragmentOperate::push_fragment_to_queue(tmp_frag, user_manager.is_user_online(member_account));
            FragmentOperate::set_msg_type(tmp_frag, USER_QUIT_FROM_GROUP);
        }
        FragmentOperate::set_receiver_account(tmp_frag, member_account);
        FragmentOperate::push_fragment_to_queue(tmp_frag, user_manager.is_user_online(member_account));
    }

    return 0;
}

int
ServerTask::remove_a_pair_of_friends_from_the_server(const std::string& friend_1_account, const std::string& friend_2_account)
{
    if (user_manager.remove_friend_from_user(friend_1_account, friend_2_account) == -1) {
        LOG_WARN("ServerTask::remove_a_pair_of_friends_from_the_server() failed: UserManager::remove_friend_from_user() calls failed: %d.", __LINE__);
        return -1;
    }

    if (user_manager.remove_friend_from_user(friend_2_account, friend_1_account) == -1) {
        LOG_WARN("ServerTask::remove_a_pair_of_friends_from_the_server() failed: UserManager::remove_friend_from_user() calls failed: %d.", __LINE__);
        return -1;
    }

    struct fragment tmp_frag;
    FragmentOperate::set_msg_type(tmp_frag, SERVER_NOTIFY_USER_THE_RESULT_OF_REMOVE_FRIEND);

    FragmentOperate::set_receiver_account(tmp_frag, friend_1_account);
    FragmentOperate::push_fragment_to_queue(tmp_frag, user_manager.is_user_online(friend_1_account));

    FragmentOperate::set_receiver_account(tmp_frag, friend_2_account);
    FragmentOperate::push_fragment_to_queue(tmp_frag, user_manager.is_user_online(friend_2_account));

    return 0;
}

int
ServerTask::remove_member_from_the_group(const std::string& group_account, const std::string& user_account)
{
    struct group group_info;
    
    if (group_manager.get_group_info(group_account, group_info) == -1) {
        LOG_WARN("ServerTask::remove_member_from_the_group() failed: GroupManager::get_group_info() calls failed.");
        return -1;;
    }

    if (group_info.group_manager_account.compare(user_account) == 0) {
        LOG_WARN("ServerTask::remove_member_from_the_group() failed: member's account is manager account.");
        return -1;
    }

    if (user_manager.remove_joined_group_from_user(user_account, group_account) == -1) {
        LOG_WARN("ServerTask::remove_member_from_the_group() failed: UserManager::remove_joined_group_from_user() calls failed.");
        return -1;
    }

    if (group_manager.remove_member_from_group(group_account, user_account) == -1) {
        LOG_WARN("ServerTask::remove_member_from_the_group() failed: GroupManager::remove_member_from_group() failed.");
        return -1;
    }

    struct fragment tmp_frag;
    FragmentOperate::set_msg_type(tmp_frag, SERVER_NOTIFY_GORUP_THE_RESULT_OF_MOVED_MEMBER);

    FragmentOperate::set_receiver_account(tmp_frag, group_info.group_manager_account);
    FragmentOperate::push_fragment_to_queue(tmp_frag, user_manager.is_user_online(user_account));

    FragmentOperate::set_receiver_account(tmp_frag, user_account);
    FragmentOperate::push_fragment_to_queue(tmp_frag, user_manager.is_user_online(user_account));

    return 0;
}

int
ServerTask::processing_tasks(void)
{
    struct fragment tmp_frag;

    if (FRAGMENT_INNER_TRANSMISSON_SIZE() > 0) {
        FRAGMENT_INNER_TRANSMISSON_POP(tmp_frag);
        switch (tmp_frag.msg_type)
        {
            case SIGN_UP:
            {
                add_a_new_user_to_the_server(tmp_frag);
                LOG_INFO("SIGN_UP successed");
            } break;
            case SIGN_IN:
            {
                std::string user_account(tmp_frag.sender_account);
                std::string user_password(tmp_frag.data);
                int ret = user_manager.check_user_password(user_account, user_password);
                if (ret == -1) {
                    LOG_WARN("ServerTask::processing_task(): check_user_password() calls failed.");
                    break;
                } else if (ret == 0) {
                    LOG_WARN("ServerTask::processing_task(): user_password is not correct.");
                    break;
                }

                d_connect_manager.add_user_connect(user_account, d_connect_manager.get_unknown_connect(tmp_frag.tmp));

                struct fragment t_user_sign_in_successed;
                FragmentOperate::set_msg_type(t_user_sign_in_successed, SIGN_IN_SUCCESSED);
                FragmentOperate::set_receiver_account(t_user_sign_in_successed, user_account);
                FragmentOperate::push_fragment_to_queue(t_user_sign_in_successed, true);

                user_manager.set_user_online(user_account);
                send_user_info(user_account);
                send_user_friend_info(user_account);
                send_user_joined_group_info(user_account);
                send_user_managered_group_info(user_account);

                std::vector<struct fragment> unsend_msgs;
                MSG_BUFFER_POP(user_account, unsend_msgs);
                for (auto msg : unsend_msgs) {
                    FragmentOperate::transmit_fragment(msg, true);
                }
                LOG_INFO("SIGN_IN: user_account: %s", tmp_frag.sender_account);
            } break;
            case SIGN_OUT:
            {
                std::string user_account(tmp_frag.sender_account);
                user_manager.set_user_offline(user_account);
                LOG_INFO("SIGN_OUT: user_account: %s", tmp_frag.sender_account);
            } break;
            case CREATE_NEW_GROUP:
            {
                // data field: group_name
                std::string user_account(tmp_frag.sender_account);
                std::string group_name(tmp_frag.data);
                add_a_new_group_to_the_server(group_name, user_account);
                send_user_joined_group_info(user_account);
                send_user_managered_group_info(user_account);
                LOG_INFO("CREATE_NEW_GROUP: new group name: %s manager: %s", group_name.c_str(), user_account.c_str());
            } break;
            case UPDATE_USER_NAME:
            {
                LOG_WARN("Update user name function not implement.");
            } break;
            case UPDATE_USER_PASSWORD:
            {
                LOG_WARN("Update user password function not implement.");
            } break;
            case UPDATE_GROUP_NAME:
            {
                LOG_WARN("Update group name function not implement.");
            } break;
            case UPDATE_GROUP_MANAGER:
            {
                LOG_WARN("Update group manager function not implement.");
            } break;
            case GROUP_MANAGER_RESPONE_JOIN_GROUP_REQUEST_AGREE:    // 服务端在发送请求消息给群管理员时， 做的只是转发, 管理员除了修改消息的类型其余都不动
            {
                add_member_to_the_group(std::string(tmp_frag.group_account), std::string(tmp_frag.sender_account));
                LOG_INFO("GROUP_MANAGER_RESPONE_JOIN_GROUP_REQUEST: group_account: %s member_account: %s", tmp_frag.group_account, tmp_frag.sender_account);
            } break;
            case GROUP_MANAGER_RESPONE_JOIN_GROUP_REQUEST_DISAGREE:
            {
                struct fragment t_notify_user_result_of_join_group;
                FragmentOperate::set_msg_type(t_notify_user_result_of_join_group, SERVER_NOTIFY_USER_THE_RESULT_OF_JOIN_GROUP_REQUEST_FAILED);
                FragmentOperate::set_receiver_account(t_notify_user_result_of_join_group, tmp_frag.receiver_account);
                FragmentOperate::push_fragment_to_queue(t_notify_user_result_of_join_group, user_manager.is_user_online(std::string(tmp_frag.receiver_account)));
            } break;
            case USER_RESPONE_FRIEND_REQUEST_AGREE:
            {
                add_a_pair_of_friends_to_the_server(std::string(tmp_frag.sender_account), std::string(tmp_frag.receiver_account));
                LOG_INFO("USER_RESPONE_FRIEND_REQUEST: friend_1: %s friend_2: %s", tmp_frag.sender_account, tmp_frag.receiver_account);
            } break;
            case USER_RESPONE_FRIEND_REQUEST_DISAGREE:
            {
                struct fragment t_notify_user_result_of_being_friend;
                FragmentOperate::set_msg_type(t_notify_user_result_of_being_friend, SERVER_NOTIFY_USER_THE_RESULT_OF_BE_FRIEND_REQUEST_FAILED);
                FragmentOperate::set_receiver_account(t_notify_user_result_of_being_friend, tmp_frag.receiver_account);
                FragmentOperate::push_fragment_to_queue(t_notify_user_result_of_being_friend, user_manager.is_user_online(std::string(tmp_frag.receiver_account)));
            } break;
            case USER_QUIT_FROM_GROUP:
            {
                remove_member_from_the_group(std::string(tmp_frag.group_account), std::string(tmp_frag.sender_account));
                LOG_INFO("USER_QUIT_FROM_GROUP: group_account: %s user_account: %s", tmp_frag.group_account, tmp_frag.sender_account);
            } break;
            case REMOVE_A_FRIEND:
            {
                remove_a_pair_of_friends_from_the_server(std::string(tmp_frag.sender_account), std::string(tmp_frag.receiver_account));
                LOG_INFO("REMOVE_A_FRIEND: friend_1: %s friend_2: %s", tmp_frag.sender_account, tmp_frag.receiver_account);
            } break;
            case GROUP_MANAGER_REMOVE_A_MEMBER:
            {
                remove_member_from_the_group(std::string(tmp_frag.group_account), std::string(tmp_frag.data));
                LOG_INFO("GROUP_MANAGER_REMOVE_A_MEMBER: group_account: %s user_account: %s", tmp_frag.group_account, tmp_frag.data);
            } break;
            case REMOVE_A_GROUP:
            {
                remove_a_group_from_server(std::string(tmp_frag.group_account));
                LOG_INFO("REMOVE_A_GROUP: group_account: %s", tmp_frag.group_account);
            } break;
            case REQUEST_USER_INFO:
            {
                send_user_info(std::string(tmp_frag.sender_account));
                LOG_INFO("REQUEST_USER_INFO: user_account: %s", tmp_frag.sender_account);
            } break;
            case REQUEST_GROUP_INFO:
            {
                send_group_info(std::string(tmp_frag.sender_account), std::string(tmp_frag.group_account));
                LOG_INFO("REQUEST_GROUP_INFO: user_account: %s group_account: %s", tmp_frag.sender_account, tmp_frag.group_account);
            } break;
            default:
                break;
        }
    }
}