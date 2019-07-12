#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "server_log.h"
#include "server_json.h"
#include "sqlite_wrapper.h"

int main(void)
{
    Json& json_instance = Json::get_json_instance("../../server_config/server.json");
    SQLiteWrapper& sql_instance = SQLiteWrapper::get_sqlitewrapper_instance(json_instance);

    std::string path = "../../server_config/tmp_user_info/";
    std::ifstream group_info(path + "group_info");
    if (!group_info.good()) {
        std::cerr << "group_info open failed." << std::endl;
        return -1;
    }

    std::ifstream group_member(path + "group_member");
    if (!group_member.good()) {
        std::cerr << "group_member open failed." << std::endl;
        return -1;
    }

    std::ifstream manager_group(path + "manager_group");
    if (!manager_group.good()) {
        std::cerr << "manager_group open failed." << std::endl;
        return -1;
    }

    std::ifstream user_friend(path + "user_friend");
    if (!user_friend.good()) {
        std::cerr << "user_friend open failed." << std::endl;
        return -1;
    }

    std::ifstream user_group(path + "user_group");
    if (!user_group.good()) {
        std::cerr << "user_group open failed." << std::endl;
        return -1;
    }

    std::ifstream user_info(path + "user_info");
    if (!user_info.good()) {
        std::cerr << "user_info open failed." << std::endl;
        return -1;
    }

    struct database_table tmp_table;
    int cnt = 0;

    tmp_table.table = GROUP_TABLE;
    while (!group_info.eof()) {
        group_info >> tmp_table.group.group_account;
        group_info >> tmp_table.group.group_name;
        group_info >> tmp_table.group.group_manager_account;
        sql_instance.insert_group_record_to_group_table(tmp_table);
        cnt++;
    }
    std::cout << "group_info all inserted, record number: " << cnt << std::endl;

    cnt = 0;
    tmp_table.table = GROUP_MEMBER_TABLE;
    while (!group_member.eof()) {
        group_member >> tmp_table.group_member.group_account;
        group_member >> tmp_table.group_member.group_member_account;
        sql_instance.insert_group_member_record_to_group_member_table(tmp_table);
        cnt++;
    }
    std::cout << "group_member all inserted, record number: " << cnt << std::endl;

    cnt = 0;
    tmp_table.table = USER_MANAGERED_GROUP_TABLE;
    while (!manager_group.eof()) {
        manager_group >> tmp_table.user_managered_group.manager_account;
        manager_group >> tmp_table.user_managered_group.managered_group_account;
        sql_instance.insert_user_managered_group_record_to_user_managered_group_table(tmp_table);
        cnt++;
    }
    std::cout << "manager_group all inserted, record number: " << cnt << std::endl;

    cnt = 0;
    tmp_table.table = USER_FRIEND_TABLE;
    while (!user_friend.eof()) {
        user_friend >> tmp_table.user_friend.user_account;
        user_friend >> tmp_table.user_friend.friend_account;
        sql_instance.insert_user_friend_record_to_user_friend_table(tmp_table);
        cnt++;
    }
    std::cout << "user_friend all inserted, record number: " << cnt << std::endl;

    cnt = 0;
    tmp_table.table = USER_JOINED_GROUP_TABLE;
    while (!user_group.eof()) {
        user_group >> tmp_table.user_joined_group.user_account;
        user_group >> tmp_table.user_joined_group.joined_group_account;
        sql_instance.insert_user_joined_group_record_to_user_joined_group_table(tmp_table);
        cnt++;
    }
    std::cout << "user_group all inserted, record number: " << cnt << std::endl;

    cnt = 0;
    tmp_table.table = USER_TABLE;
    while (!user_info.eof()) {
        user_info >> tmp_table.user.user_account;
        user_info >> tmp_table.user.user_name;
        user_info >> tmp_table.user.user_password;
        sql_instance.insert_user_record_to_user_table(tmp_table);
        cnt++;
    }
    std::cout << "user_info all inserted, record number: " << cnt << std::endl;

    return 0;
}