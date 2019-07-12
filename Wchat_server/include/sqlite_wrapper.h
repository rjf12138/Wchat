#ifndef __USER_DATA_H__
#define __USER_DATA_H__

#include <string>
#include <vector>
#include <sqlite3.h>

#include "server_json.h"
#include "server_log.h"
#include "msg_fragment.h"

#define DATABASE_FILE_PATH                  "user_database_file_path"
#define FRIENDS_UPPER_LIMIT                 "max_friend_a_user_can_hold"
#define GROUP_MEMBER_UPPER_LIMIT            "max_member_a_group_can_hold"


// 数据库表的类型
enum table_type {
    USER_TABLE,
    USER_FRIEND_TABLE,
    GROUP_TABLE,
    GROUP_MEMBER_TABLE,
    USER_JOINED_GROUP_TABLE,
    USER_MANAGERED_GROUP_TABLE
};

struct database_table {
    table_type table;               // table 表示传入数据对应的什么表
    struct {
        std::string user_account;
        std::string user_name;
        std::string user_password;
    } user;                         // 用户数据表

    struct {
        std::string user_account;
        std::string friend_account;
    } user_friend;                  // 用户好友表

    struct {
        std::string group_account;
        std::string group_name;
        std::string group_manager_account;
    } group;                        // 群表

    struct {
        std::string group_account;
        std::string group_member_account;
    } group_member;                 // 群和群成员表

    struct {
        std::string user_account;
        std::string joined_group_account;
    } user_joined_group;            // 用户所加入的群

    struct {
        std::string manager_account;
        std::string managered_group_account;
    } user_managered_group;         // 用户管理的群 （群主）
};

class SQLiteWrapper {
public:
    static SQLiteWrapper& get_sqlitewrapper_instance(Json &json_instance);                              // 返回一个 SQLiteWrapper 实例

    int is_user_exists(const std::string user_account);                                                 // 查询用户是否存在
    int is_user_friend_exists(const std::string user_account, const std::string friend_account);        // 查询用户的好友是否存在
    int is_group_exists(const std::string group_account);                                               // 群是否存在
    int is_group_member_exists(const std::string group_account, const std::string member_account);      // 群成员是否存在
    int check_user_passwd(const std::string user_account, const std::string passwd);                    // 检查用户密码是否正确

    int insert_user_record_to_user_table(struct database_table &table);                                 // 插入一条用户数据
    int insert_user_friend_record_to_user_friend_table(struct database_table &table);                   // 插入一条用户和好友的数据
    int insert_user_joined_group_record_to_user_joined_group_table(struct database_table &table);       // 插入一条用户参加的群数据
    int insert_group_record_to_group_table(struct database_table &table);                               // 插入一条群数据
    int insert_group_member_record_to_group_member_table(struct database_table &table);                 // 插入一条群成员数据
    int insert_user_managered_group_record_to_user_managered_group_table(struct database_table &table); // 插入一条用户是群管理员的数据

    int select_user_from_user_table(const std::string user_account, struct database_table &table);      // 从数据库中的用户表中读取用户信息
    int select_all_users_from_user_table(std::vector<struct database_table> &users);                    // 从数据库中读取所有用户的信息
    int select_group_from_group_table(const std::string group_account, struct database_table &table);   // 从数据库中读取群的信息
    int select_all_groups_from_group_table(std::vector<struct database_table> &groups);                 // 从数据库中读取所有群的信息
    int select_users_friends(const std::string user_account, std::vector<std::string> &friends);        // 从数据库中读取用户所有好友账号
    int select_groups_members(const std::string group_account, std::vector<std::string> &members);      // 从数据库中读取群的所有成员
    int select_all_user_managered_groups(const std::string manager_account, std::vector<std::string> &groups);  //获取所有用户管理的群账号
    int select_all_user_jonined_groups(const std::string user_account, std::vector<std::string> &groups);       //获取所有用户加入的群账号

    int update_user_name_table(struct database_table &table);           // 更新用户的用户名
    int update_user_passwd_table(struct database_table &table);         // 更新用户的密码
    int update_group_manager_table(struct database_table &table);       // 更新群组的管理员
    int update_group_name_table(struct database_table &table);          // 更新群的名字

    int delete_friend_from_user_friend_table(const std::string user_account, const std::string friend_account);            // 将好友从用户的好友列表中移除
    int delete_member_from_group_member_table(const std::string group_account, const std::string member_account);          // 将群成员从群成员列表中移除
    int delete_group_from_group_table(const std::string group_account);                                                    // 将群从数据库中移除
    int delete_group_from_user_managered_group_table(const std::string manager_account, const std::string group_account);  // 将群从用户管理的群列表中移除
    int delete_group_from_user_joined_group_table(const std::string user_account, const std::string group_account);        // 将群从用户加入的群列表中移除

private:
    SQLiteWrapper() = default;
    SQLiteWrapper(Json &json_instance);
    ~SQLiteWrapper();
    int init_database_tables(void);
private:
    sqlite3 *user_database;
    char *err_msg;
};

#endif