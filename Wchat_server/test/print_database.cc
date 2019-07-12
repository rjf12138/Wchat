#include <iostream>
#include <string>

#include "server_log.h"
#include "server_json.h"
#include "sqlite_wrapper.h"

int main(int argc, char **argv)
{
    Json& json_instance = Json::get_json_instance("../../server_config/server.json");
    SQLiteWrapper& sqlite_instance = SQLiteWrapper::get_sqlitewrapper_instance(json_instance);

    std::vector<struct database_table> tables;
    std::cout << "--------------------------user data--------------------------" << std::endl;
    sqlite_instance.select_all_users_from_user_table(tables);
    std::cout << "user_name\t\tuser_account\t\tuser_password" << std::endl;
    for (int i = 0; i < tables.size(); ++i) { 
        std::cout << tables[i].user.user_name << "\t\t\t";
        std::cout << tables[i].user.user_account << "\t\t\t" << tables[i].user.user_password << std::endl;
    }

    std::cout << "-----------------------user-friend data-----------------------" << std::endl;
    std::vector<std::string> friends;
    std::cout << "user_account\t\tfriend_account" << std::endl;
    for (int j = 0; j < tables.size(); ++j) {
        sqlite_instance.select_users_friends(tables[j].user.user_account, friends);
        for (int i = 0; i < friends.size(); ++i) { 
            std::cout << tables[j].user.user_account << "\t\t\t" << friends[i] << std::endl;
        }
    }

    std::cout << "--------------------------group data--------------------------" << std::endl;
    std::cout << "group_name\t\tgroup_account\t\tgroup_manager" << std::endl;
    sqlite_instance.select_all_groups_from_group_table(tables);
    for (int i = 0; i < tables.size(); ++i) { 
        std::cout << tables[i].group.group_name << "\t\t\t";
        std::cout << tables[i].group.group_account << "\t\t\t" << tables[i].group.group_manager_account << std::endl;
    }

    std::cout << "----------------------group-member data-----------------------" << std::endl;
    std::vector<std::string> members;
    std::cout << "group_account\t\tmember_account" << std::endl;
    for (int j = 0; j < tables.size(); ++j) { 
        sqlite_instance.select_groups_members(tables[j].group.group_account, members);
        for (int i = 0; i < members.size(); ++i) { 
            std::cout << tables[j].group.group_account << "\t\t\t" << members[i] << std::endl;
        }
    }

    return 0;
}