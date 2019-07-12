#include <vector>
#include <ctime>

#include "user_manager.h"

void 
UserManager::check_heartbeart(int sig) 
{
/*
    check_all_user_heart_beat();
    alarm(user_manager_instance.check_heartbeart_time);
*/ 
// 将上面这两条移到线程里去处理，这个回调函数只是通知线程可以这么处理了
}

UserManager&
UserManager::get_user_manager_instance(SQLiteWrapper &sqlite_instance, Json &json_instance)
{
    static UserManager user_manager_instance(sqlite_instance, json_instance);
    
    return user_manager_instance;
}

UserManager::UserManager(SQLiteWrapper &sqlite_instance, Json &json_instance): sqlite_wrapper(sqlite_instance)
{
    json_instance.read_value(std::string(MAX_HEART_TIME), max_heartbeat_time);
    json_instance.read_value(std::string(CHECK_HEART_TIME_GAP), check_heartbeart_time);

    if (signal(SIGALRM, check_heartbeart) == SIG_ERR) {
        LOG_ERROR("UserManager:: get_user_manager_instance() failed: signal() call failed.");
    }

    std::vector<struct database_table> users;
    if (sqlite_instance.select_all_users_from_user_table(users) == -1) {
        LOG_ERROR("UserManager:: UserManager constructor failed: SQLiteWrapper::select_all_users_from_user_table calls failed.");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < users.size(); ++i) {
        struct user tmp_user;
        tmp_user.user_account = users[i].user.user_account;
        tmp_user.user_name = users[i].user.user_name;
        tmp_user.online = USER_OFFLINE;
        tmp_user.heart_beat = 0;
        std::vector<std::string> friends;
        if (sqlite_instance.select_users_friends(tmp_user.user_account, friends) == -1) {
            LOG_ERROR("UserManager:: UserManager constructor failed: SQLiteWrapper::select_all_users_from_user_table calls failed.");
            exit(EXIT_FAILURE);
        }
        for (int j = 0; j < friends.size(); ++j) {
            tmp_user.user_friends.push_back(friends[j]);
        }
        std::vector<std::string> user_managered_groups;
        if (sqlite_instance.select_all_user_managered_groups(tmp_user.user_account, user_managered_groups) == -1) {
            LOG_ERROR("UserManager:: UserManager constructor failed: SQLiteWrapper::select_all_user_managered_groups calls failed.");
            exit(EXIT_FAILURE);
        }
        for (int j = 0; j < user_managered_groups.size(); ++j) {
            tmp_user.user_managered_groups.push_back(user_managered_groups[j]);
        }
        std::vector<std::string> user_joined_groups;
        if (sqlite_instance.select_all_user_jonined_groups(tmp_user.user_account, user_joined_groups) == -1) {
            LOG_ERROR("UserManager:: UserManager constructor failed: SQLiteWrapper::select_all_user_joined_groups calls failed.");
            exit(EXIT_FAILURE);
        }
        for (int j = 0; j < user_joined_groups.size(); ++j) {
            tmp_user.user_joined_groups.push_back(user_joined_groups[j]);
        }
        manager[tmp_user.user_account] = tmp_user;
    }

    alarm(check_heartbeart_time); //开启用户定时
}

int 
UserManager::is_user_online(const std::string &user_account)
{
    auto user_iter = manager.find(user_account);
    if (user_iter == manager.end()) {
        LOG_WARN("UserManager:: is_user_online() failed: %s is not exists.", user_account.c_str());
        return -1;
    }

    if (user_iter->second.online == true) {
        return USER_ONLINE;
    } else if (user_iter->second.online == false) {
        return USER_OFFLINE;
    }
}

int
UserManager::is_user_exists(const std::string& user_account)
{
    auto user_iter = manager.find(user_account);
    if (user_iter == manager.end()) {
        return 0;
    }
    return 1;
}

int 
UserManager::check_user_password(const std::string &user_account, const std::string &password)
{
    return sqlite_wrapper.check_user_passwd(user_account, password);
}

int
UserManager::add_new_user(struct user &new_user)
{
    struct database_table user;
    user.user.user_account = new_user.user_account;
    user.user.user_name = new_user.user_name;
    user.user.user_password = new_user.user_password;
    if (sqlite_wrapper.insert_user_record_to_user_table(user) == -1) {
        LOG_WARN("UserManager:: add_new_user() failed: %s add failed.", new_user.user_account);
        return -1;
    }

    new_user.online = USER_OFFLINE;
    new_user.heart_beat = 0;
    new_user.user_password = "";
    manager[new_user.user_account] = new_user;
    return 0;
}

int 
UserManager::set_user_online(const std::string &user_account)
{
    auto user_iter = manager.find(user_account);
    if (user_iter == manager.end()) {
        LOG_WARN("UserManager:: set_user_online() failed: %s is not exists.", user_account.c_str());
        return -1;
    }

    user_iter->second.online = USER_ONLINE;
    return 0;
}
int 
UserManager::set_user_offline(const std::string &user_account)
{
    auto user_iter = manager.find(user_account);
    if (user_iter == manager.end()) {
        LOG_WARN("UserManager:: set_user_offline() failed: %s is not exists.", user_account.c_str());
        return -1;
    }

    user_iter->second.online = USER_OFFLINE;
    return 0;
}

int 
UserManager::add_friend_to_user(const std::string &user_account, const std::string &friend_account)
{
    auto user_iter = manager.find(user_account);
    if (user_iter == manager.end()) {
        LOG_WARN("UserManager:: add_friend_to_user() failed: %s is not exists.", user_account.c_str());
        return -1;
    }

    user_iter->second.user_friends.push_back(friend_account);
    struct database_table tmp_friend;
    tmp_friend.user_friend.user_account = user_account;
    tmp_friend.user_friend.friend_account = friend_account;
    tmp_friend.table = USER_FRIEND_TABLE;
    sqlite_wrapper.insert_user_friend_record_to_user_friend_table(tmp_friend);
    return 0;
}

int
UserManager::add_managered_group_to_user(const std::string &user_account, const std::string &group_account)
{
    auto user_iter = manager.find(user_account);
    if (user_iter == manager.end()) {
        LOG_WARN("UserManager:: add_managered_group_to_user() failed: %s is not exists.", user_account.c_str());
        return -1;
    }
    user_iter->second.user_managered_groups.push_back(group_account);

    struct database_table tmp;
    tmp.table = USER_MANAGERED_GROUP_TABLE;
    tmp.user_managered_group.manager_account = user_account;
    tmp.user_managered_group.managered_group_account = group_account;
    sqlite_wrapper.insert_user_managered_group_record_to_user_managered_group_table(tmp);

    return 0;
}

int 
UserManager::add_joined_group_to_user(const std::string &user_account, const std::string &group_account)
{
    auto user_iter = manager.find(user_account);
    if (user_iter == manager.end()) {
        LOG_WARN("UserManager:: add_joined_group_to_user() failed: %s is not exists.", user_account.c_str());
        return -1;
    }
    user_iter->second.user_joined_groups.push_back(group_account);

    struct database_table tmp;
    tmp.table = USER_JOINED_GROUP_TABLE;
    tmp.user_joined_group.user_account = user_account;
    tmp.user_joined_group.joined_group_account = group_account;
    sqlite_wrapper.insert_user_joined_group_record_to_user_joined_group_table(tmp);

    return 0;
}

int 
UserManager::remove_friend_from_user(const std::string &user_account, const std::string &friend_account)
{
    auto user_iter = manager.find(user_account);
    if (user_iter == manager.end()) {
        LOG_WARN("UserManager:: remove_friend_from_user() failed: user %s is not exists.", user_account.c_str());
        return -1;
    }

    auto friend_iter = std::find(user_iter->second.user_friends.begin(), user_iter->second.user_friends.end(), friend_account);
    if (friend_iter == user_iter->second.user_friends.end()) {
        LOG_WARN("UserManager:: remove_friend_from_user() failed: friend %s is not exists.", friend_account.c_str());
        return -1;
    }

    sqlite_wrapper.delete_friend_from_user_friend_table(user_account, friend_account);
    user_iter->second.user_friends.erase(friend_iter);

    return 0;
}

int 
UserManager::remove_managered_group_from_user(const std::string& user_account, const std::string& group_account)
{
    auto user_iter = manager.find(user_account);
    if (user_iter == manager.end()) {
        LOG_WARN("UserManager:: remove_managered_group_from_user() failed: user %s is not exists.", user_account.c_str());
        return -1;
    }

    auto group_iter = std::find(user_iter->second.user_managered_groups.begin(), user_iter->second.user_managered_groups.end(), group_account);
    if (group_iter == user_iter->second.user_managered_groups.end()) {
        LOG_WARN("UserManager:: remove_managered_group_from_user() failed: group_account %s is not exists.", group_account.c_str());
        return -1;
    }

    sqlite_wrapper.delete_group_from_user_managered_group_table(user_account, group_account);
    return 0;
}

int 
UserManager::remove_joined_group_from_user(const std::string& user_account, const std::string& group_account)
{
    auto user_iter = manager.find(user_account);
    if (user_iter == manager.end()) {
        LOG_WARN("UserManager:: remove_joined_group_from_user() failed: user %s is not exists.", user_account.c_str());
        return -1;
    }

    auto group_iter = std::find(user_iter->second.user_joined_groups.begin(), user_iter->second.user_joined_groups.end(), group_account);
    if (group_iter == user_iter->second.user_joined_groups.end()) {
        LOG_WARN("UserManager:: remove_joined_group_from_user() failed: grouop_account %s is not exists.", group_account.c_str());
        return -1;
    }

    sqlite_wrapper.delete_group_from_user_joined_group_table(user_account, group_account);
    return 0;
}

int 
UserManager::get_user_info(const std::string& user_account, struct user &user_info)
{
    auto user_iter = manager.find(user_account);
    if (user_iter == manager.end()) {
        LOG_WARN("UserManager:: get_user_info() failed: user account %s is not exists.", user_account.c_str());
        return -1;
    }

    user_info.user_account = user_iter->second.user_account;
    user_info.user_name = user_iter->second.user_name;
    return 0;
}

int 
UserManager::get_all_user_friends_account(const std::string &user_account, std::vector<std::string> &friends)
{
    auto user_iter = manager.find(user_account);
    if (user_iter == manager.end()) {
        LOG_WARN("UserManager:: get_all_user_friends_account() failed: user %s is not exists.", user_account.c_str());
        return -1;
    }

    auto friend_begin = user_iter->second.user_friends.begin();
    auto friend_end = user_iter->second.user_friends.end();
    friends.clear();
    while (friend_begin != friend_end) {
        friends.push_back(*friend_begin);
        ++friend_begin;
    }
    return 0;
}
int 
UserManager::get_all_user_managered_groups_account(const std::string &user_account, std::vector<std::string> &groups)
{
    auto user_iter = manager.find(user_account);
    if (user_iter == manager.end()) {
        LOG_WARN("UserManager:: get_all_user_managered_groups_account() failed: user %s is not exists.", user_account.c_str());
        return -1;
    }

    auto group_begin = user_iter->second.user_managered_groups.begin();
    auto group_end = user_iter->second.user_managered_groups.end();
    groups.clear();
    while (group_begin != group_end) {
        groups.push_back(*group_begin);
        ++group_begin;
    }
    return 0;
}
int
UserManager::get_all_user_joined_groups_account(const std::string &user_account, std::vector<std::string> &groups)
{
    auto user_iter = manager.find(user_account);
    if (user_iter == manager.end()) {
        LOG_WARN("UserManager:: get_all_user_joined_groups_account() failed: user %s is not exists.", user_account.c_str());
        return -1;
    }

    auto group_begin = user_iter->second.user_joined_groups.begin();
    auto group_end = user_iter->second.user_joined_groups.end();
    groups.clear();
    while (group_begin != group_end) {
        groups.push_back(*group_begin);
        ++group_begin;
    }
    return 0;
}

int 
UserManager::get_user_name_according_to_the_user_account(const std::vector<std::string>& user_accounts, std::vector<std::string>& user_names)
{
    user_names.clear();
    for (const auto& account : user_accounts) {
        auto user_iter = manager.find(account);
        if (user_iter == manager.end()) {
            LOG_WARN("UserManager:: get_user_name_according_to_the_user_account() failed: user %s is not exists.", account.c_str());
            continue;
        }
        user_names.push_back(user_iter->second.user_name);
    }
    return 0;
}

int 
UserManager::get_user_address(const std::string &user_account, struct sockaddr_in &addr)
{
    auto user_iter = manager.find(user_account);
    if (user_iter == manager.end()) {
        LOG_WARN("UserManager:: get_user_address() failed: user %s is not exists.", user_account.c_str());
        return -1;
    }

    addr = user_iter->second.user_addr;
    return 0;
}

int
UserManager::update_user_name(const std::string& user_account, const std::string &name)
{
    auto user_iter = manager.find(user_account);
    if (user_iter == manager.end()) {
        LOG_WARN("UserManager:: update_user_name() failed: user %s is not exists.", user_account.c_str());
        return -1;
    }

    user_iter->second.user_name = name;
    struct database_table tmp_user;
    tmp_user.table = USER_TABLE;
    tmp_user.user.user_account = user_account;
    tmp_user.user.user_name = name;
    sqlite_wrapper.update_user_name_table(tmp_user);
    return 0;
}

int
UserManager::update_user_password(const std::string& user_account, const std::string& new_password)
{
    struct database_table tmp_user;
    tmp_user.table = USER_TABLE;
    tmp_user.user.user_account = user_account;
    tmp_user.user.user_password = new_password;
    sqlite_wrapper.update_user_passwd_table(tmp_user);
    return 0;
}

int 
UserManager::update_user_address(const std::string &user_account, struct sockaddr_in &client_addr)
{
    auto user_iter = manager.find(user_account);
    if (user_iter == manager.end()) {
        LOG_WARN("UserManager:: update_user_address() failed: user %s is not exists.", user_account.c_str());
        return -1;
    }

    user_iter->second.user_addr = client_addr;
    return 0;
}

int 
UserManager::update_user_heartbeat_time(const std::string &user_account)
{
    auto user_iter = manager.find(user_account);
    if (user_iter == manager.end()) {
        LOG_WARN("UserManager:: update_user_heartbeat_time() failed: user %s is not exists.", user_account.c_str());
        return -1;
    }

    user_iter->second.heart_beat = time(NULL);
    return 0;
}
int 
UserManager::count_online_user(void)
{
    int cnt_online_user = 0;
    auto iter = manager.begin();
    for (; iter != manager.end(); ++iter) {
        if (iter->second.online == USER_ONLINE) {
            ++cnt_online_user;
        }
    }
    return cnt_online_user;
}
int 
UserManager::check_all_user_heart_beat(void)
{
    for (auto iter = manager.begin(); iter != manager.end(); ++iter) {
        time_t now = time(NULL);
        if (now - iter->second.heart_beat > max_heartbeat_time) {
            LOG_INFO("OnlineUser:: check_all_user_heart_beat(): User %s is offline.", iter->first.c_str());
            iter->second.online = USER_OFFLINE;
        }
    }
    return 0;
}