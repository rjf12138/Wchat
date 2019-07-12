#include "server_log.h"
#include "server_json.h"
#include "user_manager.h"
#include "group_manager.h"


int main(void)
{
    Json& json_instance = Json::get_json_instance("../../server_config/server.json");
    SQLiteWrapper& sqlite_instance = SQLiteWrapper::get_sqlitewrapper_instance(json_instance);
    UserManager& user_instance = UserManager::get_user_manager_instance(sqlite_instance, json_instance);
    GroupManager& group_instance = GroupManager::get_group_manager_instance(sqlite_instance);

    std::vector<struct database_table> users;
    std::vector<std::string> tmp_info;
    sqlite_instance.select_all_users_from_user_table(users);
    std::cout << "user_count: " << users.size() << std::endl;
    for (auto &tmp : users) {
        if (user_instance.is_user_exists(tmp.user.user_account) == 1) {
            std::cout << "user: " << tmp.user.user_account << " is exists." << std::endl;
        } else {
            std::cout << "user: " << tmp.user.user_account << " is not exists." << std::endl;
        }
    }

    std::cout << std::endl;
    for (auto &tmp : users) {
        std::cout << "user: " << tmp.user.user_account << " joined groups." << std::endl;
        user_instance.get_all_user_joined_groups_account(tmp.user.user_account, tmp_info);
        for (int i = 0; i < tmp_info.size(); ++i) {
            std::cout << tmp_info[i] << std::endl;
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;
    for (auto &tmp : users) {
        std::cout << "user: " << tmp.user.user_account << " managered groups." << std::endl;
        user_instance.get_all_user_managered_groups_account(tmp.user.user_account, tmp_info);
        for (int i = 0; i < tmp_info.size(); ++i) {
            std::cout << tmp_info[i] << std::endl;
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;
    for (auto &tmp : users) {
        std::cout << "user: " << tmp.user.user_account << " friends." << std::endl;
        user_instance.get_all_user_friends_account(tmp.user.user_account, tmp_info);
        for (int i = 0; i < tmp_info.size(); ++i) {
            std::cout << tmp_info[i] << std::endl;
        }
        std::cout << std::endl;
    }

    for (int i = 0; i < users.size(); i += 2) {
        user_instance.set_user_online(users[i].user.user_account);
        std::cout << i << " ";
    }
    std::cout << std::endl;
    for (int i = 0; i < users.size(); ++i) {
        if (user_instance.is_user_online(users[i].user.user_account)) {
            std::cout << "user: " << users[i].user.user_account << " is online." << std::endl;
        } else {
            std::cout << "user: " << users[i].user.user_account << " is offline." << std::endl;
        }
    }
    std::cout << std::endl;
    for (int i = 0; i < users.size(); i += 2) {
        user_instance.set_user_offline(users[i].user.user_account);
    }

    for (int i = 0; i < users.size(); ++i) {
        if (user_instance.is_user_online(users[i].user.user_account)) {
            std::cout << "user: " << users[i].user.user_account << " is online." << std::endl;
        } else {
            std::cout << "user: " << users[i].user.user_account << " is offline." << std::endl;
        }
    }
    std::cout << std::endl;

    struct database_table new_user;
    struct database_table tmp_user;
    struct user tmp_user_1;
    tmp_user_1.user_account = "100020";
    tmp_user_1.user_name = "new_user";
    tmp_user_1.user_password = "123456tyu";
    user_instance.add_new_user(tmp_user_1);
    sqlite_instance.select_user_from_user_table("100020", tmp_user);
    user_instance.get_user_info("100020", tmp_user_1);

    std::cout << "user_account: " << tmp_user.user.user_account << " user_name: " << tmp_user.user.user_name  << " user password: " << tmp_user.user.user_password<< std::endl;
    std::cout << "user_account: " << tmp_user_1.user_account << " user_name: " << tmp_user_1.user_name << std::endl;

    user_instance.add_friend_to_user("100019", "100020");
    user_instance.add_friend_to_user("100020", "100019");
    std::cout << std::endl;
    std::cout << "user: 100019" << " friends." << std::endl;
    user_instance.get_all_user_friends_account("100019", tmp_info);
    for (int i = 0; i < tmp_info.size(); ++i) {
        std::cout << tmp_info[i] << std::endl;
    }

    std::cout << std::endl;
    std::cout << "user: 100020" << " friends." << std::endl;
    user_instance.get_all_user_friends_account("100020", tmp_info);
    for (int i = 0; i < tmp_info.size(); ++i) {
        std::cout << tmp_info[i] << std::endl;
    }
    user_instance.remove_friend_from_user("100019", "100020");
    user_instance.remove_friend_from_user("100020", "100019");

    std::cout << std::endl;
    std::cout << "user: 100019" << " friends." << std::endl;
    user_instance.get_all_user_friends_account("100019", tmp_info);
    for (int i = 0; i < tmp_info.size(); ++i) {
        std::cout << tmp_info[i] << std::endl;
    }

    std::cout << std::endl;
    std::cout << "user: 100020" << " friends." << std::endl;
    user_instance.get_all_user_friends_account("100020", tmp_info);
    for (int i = 0; i < tmp_info.size(); ++i) {
        std::cout << tmp_info[i] << std::endl;
    }

    struct group new_group;
    new_group.group_account = "10020";
    new_group.group_name = "ikun_heart";
    new_group.group_manager_account = "100018";
    group_instance.add_new_group(new_group);
    user_instance.add_joined_group_to_user("100018", "10020");
    user_instance.add_managered_group_to_user("100018", "10020");
    group_instance.add_member_to_group("10020", "100018");

    if (sqlite_instance.is_group_exists("10020") == 1) {
        std::cout << "group: 10020 is exists." << std::endl;
    } else {
        std::cout << "group: 10020 is not exists." << std::endl;
    }

    std::cout << "user: 100018 joined groups." << std::endl;
    user_instance.get_all_user_joined_groups_account("100018", tmp_info);
    for (int i = 0; i < tmp_info.size(); ++i) {
        std::cout << tmp_info[i] << std::endl;
    }
    std::cout << std::endl;

    std::cout << "user: 100018 managered groups." << std::endl;
    user_instance.get_all_user_managered_groups_account("100018", tmp_info);
    for (int i = 0; i < tmp_info.size(); ++i) {
        std::cout << tmp_info[i] << std::endl;
    }
    std::cout << std::endl;

    struct group tmp_group;
    group_instance.get_group_info("10020", tmp_group);
    std::cout << "The manager of group 10020 is " << tmp_group.group_account << std::endl;
    std::cout << "The name of group 10020 is " << tmp_group.group_name << std::endl;

    group_instance.add_member_to_group("10020", "100010");
    user_instance.add_joined_group_to_user("100010", "10020");

    std::cout << "user: 100010 joined groups." << std::endl;
    user_instance.get_all_user_joined_groups_account("100010", tmp_info);
    for (int i = 0; i < tmp_info.size(); ++i) {
        std::cout << tmp_info[i] << std::endl;
    }
    std::cout << std::endl;

    std::cout << "group: 10020 members." << std::endl;
    group_instance.get_group_info("10020", tmp_group);
    for (int i = 0; i < tmp_group.member_accounts.size(); ++i) {
        std::cout << tmp_group.member_accounts[i] << std::endl;
    }
    std::cout << std::endl;

    return 0;
}