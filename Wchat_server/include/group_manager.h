#ifndef __GROUP_MANAGER_H__
#define __GROUP_MANAGER_H__

#include <list>
#include <map>
#include <cerrno>
#include <string>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>


#include "sqlite_wrapper.h"
#include "server_json.h"
#include "server_log.h"

struct group {
    std::string group_account;
    std::string group_name;
    std::string group_manager_account;
    std::vector<std::string> member_accounts;
};

class GroupManager {
public:
    static GroupManager& get_group_manager_instance(SQLiteWrapper& sqlite_instance);            // 创建一个群的实例
    
    int get_group_info(const std::string& group_account, struct group& group_info);             // 获取群组的信息
    int get_all_groups_info(std::vector<struct group>& groups_info);                            // 获取所有群组的信息
    int get_group_name_according_to_the_group_account(const std::vector<std::string>& group_accounts, std::vector<std::string>& group_names);   // 根据群名称获取群账号

    int add_new_group(struct group &new_group);                    // 创建一个新的群组
    int add_member_to_group(const std::string& group_account, const std::string& member_account);   // 向群中加入一个用户
    int remove_member_from_group(const std::string& group_account, const std::string& member_account);  // 从群中移除一个用户
    int update_group_manager(const std::string& group_account, const std::string& new_manager_account); // 更新群的管理员
    int update_group_name(const std::string& group_account, const std::string& new_group_name);         // 更新群的名称
    int remove_group(const std::string& group_account);             // 移除一个群
private:
    GroupManager() = default;
    GroupManager(SQLiteWrapper& sqlite_instance);
    ~GroupManager() = default;
private:
    SQLiteWrapper& sqlite_wrapper;
    std::map<std::string, struct group> manager;
};

#endif