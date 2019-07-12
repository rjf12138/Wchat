#ifndef __USER_MANAGER_H__
#define __USER_MANAGER_H__

#include <list>
#include <map>
#include <cerrno>
#include <string>
#include <cstdlib>
#include <csignal>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>


#include "sqlite_wrapper.h"
#include "server_json.h"
#include "server_log.h"

#define MAX_HEART_TIME          "online_user_heartbeat_time"
#define CHECK_HEART_TIME_GAP    "check_heartbeat_time_gap"

#define USER_ONLINE     true
#define USER_OFFLINE    false

struct user {
    bool online;
    std::string user_account;
    std::string user_name;
    std::string user_password;
    struct sockaddr_in user_addr;
    int heart_beat;
    std::vector<std::string> user_friends;
    std::vector<std::string> user_managered_groups;
    std::vector<std::string> user_joined_groups;
};

class UserManager {
public:
    static UserManager& get_user_manager_instance(SQLiteWrapper &sqlite_instance, Json &json_instance);
    int is_user_online(const std::string &user_account);
    int set_user_online(const std::string &user_account);
    int set_user_offline(const std::string &user_account);
    int is_user_exists(const std::string &user_account);
    int check_user_password(const std::string &user_account, const std::string &password);

    int add_new_user(struct user &new_user);
    int add_friend_to_user(const std::string &user_account, const std::string &friend_account);
    int add_managered_group_to_user(const std::string &user_account, const std::string &group_account);
    int add_joined_group_to_user(const std::string &user_account, const std::string &group_account);
    int update_user_name(const std::string &user_account, const std::string &new_name);
    int update_user_password(const std::string &user_account, const std::string &new_password);
    int remove_friend_from_user(const std::string &user_account, const std::string &friend_account);
    int remove_managered_group_from_user(const std::string& user_account, const std::string& group_account);
    int remove_joined_group_from_user(const std::string& user_account, const std::string& group_account);
    int get_user_info(const std::string& user_account, struct user &user_info);
    int get_all_user_friends_account(const std::string &user_account, std::vector<std::string> &friends);
    int get_all_user_managered_groups_account(const std::string &user_account, std::vector<std::string> &groups);
    int get_all_user_joined_groups_account(const std::string &user_account, std::vector<std::string> &groups);
    int get_user_name_according_to_the_user_account(const std::vector<std::string>& user_accounts, std::vector<std::string>& user_names);

    int get_user_address(const std::string &user_account, struct sockaddr_in &addr);
    int update_user_address(const std::string &user_account, struct sockaddr_in &client_addr);

    int update_user_heartbeat_time(const std::string &user_account);
    int count_online_user(void);
    int check_all_user_heart_beat(void);       //TODO

private:
    UserManager(SQLiteWrapper &sqlite_instance,Json &json_instance);   //构造函数中获取所有在数据库中的数据
    static void check_heartbeart(int sig);
    ~UserManager() = default;
private:
    int max_heartbeat_time;             // 在线用户未回应服务端最大在现时长
    int check_heartbeart_time;          // 服务端每次检查时间间隔
    SQLiteWrapper &sqlite_wrapper;
    std::map<std::string, struct user> manager;
};


#endif