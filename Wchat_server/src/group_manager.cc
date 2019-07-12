#include "group_manager.h"

GroupManager&
GroupManager::get_group_manager_instance(SQLiteWrapper& sqlite_instance)
{
    static GroupManager group_manager_instance(sqlite_instance);
    return group_manager_instance;
}

GroupManager::GroupManager(SQLiteWrapper& sqlite_instance): sqlite_wrapper(sqlite_instance)
{
    std::vector<struct database_table> groups;
    if (sqlite_wrapper.select_all_groups_from_group_table(groups) == -1) {
        LOG_ERROR("GroupManager:: GroupManager() failed: select_all_groups_from_group_table calls failed.");
        exit(EXIT_FAILURE);
    }

    struct group tmp_group;
    for (int i = 0; i < groups.size(); ++i) {
        tmp_group.group_account = groups[i].group.group_account;
        tmp_group.group_name = groups[i].group.group_name;
        tmp_group.group_manager_account = groups[i].group.group_manager_account;
        std::vector<std::string> members;
        if (sqlite_wrapper.select_groups_members(tmp_group.group_account, members) == -1) {
            LOG_WARN("GroupManager:: GroupManager() failed: select_groups_members calls failed.");
            exit(EXIT_FAILURE);
        }
        for (int i = 0; i < members.size(); ++i) {
            tmp_group.member_accounts.push_back(members[i]);
        }
        manager[tmp_group.group_account] = tmp_group;
    }
}

int 
GroupManager::get_group_info(const std::string &group_account, struct group& group_info)
{
    auto group_iter = manager.find(group_account);
    if (group_iter == manager.end()) {
        LOG_WARN("GroupManager:: get_group_info(): the group account of %s is not exists.", group_account.c_str());
        return -1;
    }
    group_info.group_account = group_iter->second.group_account;
    group_info.group_manager_account = group_iter->second.group_manager_account;
    group_info.group_name = group_iter->second.group_name;
    
    group_info.member_accounts.clear();
    for (const auto &tmp : group_iter->second.member_accounts) {
        group_info.member_accounts.push_back(tmp);
    }
    return 0;
}

int 
GroupManager::get_all_groups_info(std::vector<struct group>& groups_info)
{
    groups_info.clear();
    for (const auto& tmp : manager) {
        struct group tmp_group;
        tmp_group.group_account = tmp.second.group_account;
        tmp_group.group_manager_account = tmp.second.group_manager_account;
        tmp_group.group_name = tmp.second.group_name;
        tmp_group.member_accounts.clear();
        std::copy(tmp.second.member_accounts.begin(), tmp.second.member_accounts.end(), tmp_group.member_accounts.begin());
        groups_info.push_back(tmp_group);
    }
    return 0;
}

int
GroupManager::get_group_name_according_to_the_group_account(const std::vector<std::string>& group_accounts, std::vector<std::string>& group_names)
{
    group_names.clear();
    for (const auto& account : group_accounts) {
        auto group_iter = manager.find(account);
        if (group_iter == manager.end()) {
            LOG_WARN("GroupManager:: get_group_name_according_to_the_group_account failed: the group %s is not exists.", account.c_str());
            continue;
        }
        group_names.push_back(group_iter->second.group_name);
    }
    return 0;
}



int
GroupManager::add_new_group(struct group &new_group)
{
    struct database_table tmp_group;
    tmp_group.table = GROUP_TABLE;
    tmp_group.group.group_account = new_group.group_account;
    tmp_group.group.group_name = new_group.group_name;
    tmp_group.group.group_manager_account = new_group.group_manager_account;
    if (sqlite_wrapper.insert_group_record_to_group_table(tmp_group) == -1) {
        LOG_WARN("GroupManager:: add_new_group() failed: insert_group_record_to_group_table() failed.");
        return -1;
    }

    manager[new_group.group_account] = new_group;
    return 0;
}

int
GroupManager::add_member_to_group(const std::string& group_account, const std::string& member_account)
{
    auto group_iter = manager.find(group_account);
    if (group_iter == manager.end()) {
        LOG_WARN("GroupManager:: add_member_to_group() failed: the group account of %s is not exists.", group_account);
        return -1;
    }

    struct database_table tmp_member;
    tmp_member.table = GROUP_MEMBER_TABLE;
    tmp_member.group_member.group_account = group_account;
    tmp_member.group_member.group_member_account = member_account;

    if (sqlite_wrapper.insert_group_member_record_to_group_member_table(tmp_member) == -1) {
        LOG_WARN("GroupManager:: add_member_to_group(): insert_group_member_record_to_group_member_table() failed.");
        return -1;
    }
    group_iter->second.member_accounts.push_back(member_account);

    return 0;
}

int
GroupManager::remove_member_from_group(const std::string& group_account, const std::string& member_account)
{
    auto group_iter = manager.find(group_account);
    if (group_iter == manager.end()) {
        LOG_WARN("GroupManager:: remove_member_from_group() failed: the group account of %s is not exists.", group_account);
        return -1;
    }

    if (sqlite_wrapper.delete_member_from_group_member_table(group_account, member_account) == -1) {
        LOG_WARN("GroupManager:: remove_member_from_group() failed: the group account of %s is not exists.", group_account);
        return -1;
    }

    auto member_iter = std::find(group_iter->second.member_accounts.begin(), group_iter->second.member_accounts.end(), member_account);
    if (member_iter == group_iter->second.member_accounts.end()) {
        LOG_WARN("GroupManager:: remove_member_from_group() failed: the member %s of the group %s is not exists.", member_account, group_account);
        return -1;
    }

    group_iter->second.member_accounts.erase(member_iter);
    return 0;
}

int
GroupManager::update_group_manager(const std::string& group_account, const std::string& new_manager_account)
{
    auto group_iter = manager.find(group_account);
    if (group_iter == manager.end()) {
        LOG_WARN("GroupManager:: update_group_manager() failed: the group account of %s is not exists.", group_account);
        return -1;
    }

    struct database_table tmp_manager;
    tmp_manager.table = GROUP_TABLE;
    tmp_manager.group.group_account = group_account;
    tmp_manager.group.group_manager_account = new_manager_account;
    if (sqlite_wrapper.update_group_manager_table(tmp_manager) == -1) {
        LOG_WARN("GroupManager:: update_group_manager() failed: update_group_manager_table calls failed.");
        return -1;
    }
    group_iter->second.group_manager_account = new_manager_account;
    return 0;
}

int
GroupManager::update_group_name(const std::string& group_account, const std::string& new_group_name)
{
    auto group_iter = manager.find(group_account);
    if (group_iter == manager.end()) {
        LOG_WARN("GroupManager:: update_group_name() failed: the group account of %s is not exists.", group_account);
        return -1;
    }

    struct database_table tmp_manager;
    tmp_manager.table = GROUP_TABLE;
    tmp_manager.group.group_account = group_account;
    tmp_manager.group.group_name = new_group_name;
    if (sqlite_wrapper.update_group_name_table(tmp_manager) == -1) {
        LOG_WARN("GroupManager:: update_group_name() failed: update_group_name_table() calls failed.");
        return -1;
    }

    group_iter->second.group_name = new_group_name;
    return 0;
}

int
GroupManager::remove_group(const std::string& group_account)
{
    auto group_iter = manager.find(group_account);
    if (group_iter == manager.end()) {
        LOG_WARN("GroupManager:: remove_group() failed: the group of %s is not exists.", group_account);
        return -1;
    }

    manager.erase(group_iter);
    return 0;
}