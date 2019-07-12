#include <memory>

#include <unistd.h>

#include "sqlite_wrapper.h"

SQLiteWrapper&
SQLiteWrapper::get_sqlitewrapper_instance(Json &json_instance)
{
    static SQLiteWrapper sqlitewrapper_instance(json_instance);
    return sqlitewrapper_instance;
}

SQLiteWrapper::SQLiteWrapper(Json &json_instance)
{
    std::string file_path;
    json_instance.read_value(DATABASE_FILE_PATH, file_path);

    int file_exist = access(file_path.c_str(), F_OK);

    int ret = sqlite3_open(file_path.c_str(), &user_database);
    if (ret != SQLITE_OK) {
        LOG_ERROR("SQLiteWrapper:: get_sqlitewrapper_instance(): %s", sqlite3_errmsg(user_database));
        exit(EXIT_FAILURE);
    } else {
        LOG_INFO("SQLiteWrapper:: open user database successful");
    }

    if (file_exist == -1) {
        init_database_tables();
    }
}

SQLiteWrapper::~SQLiteWrapper()
{
    sqlite3_close(user_database);
}

int 
SQLiteWrapper::is_user_exists(const std::string user_account)
{
    char sql[256] = {0};
    sqlite3_stmt *stmt = nullptr;
    const char *ztail = nullptr;
    sprintf(sql, "SELECT user_account,user_name FROM user_table WHERE user_account = '%s'", user_account.c_str());
    int ret = sqlite3_prepare_v2(user_database, sql, -1, &stmt, &ztail);
    if (ret != SQLITE_OK) {
        LOG_ERROR("SQLiteWrapper:: select_user_from_user_table calls failed: %s", sqlite3_errmsg(user_database));
        return -1;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        if (strncmp(user_account.c_str(), (const char *)sqlite3_column_text(stmt, 0), strlen(user_account.c_str())) == 0) {
            sqlite3_finalize(stmt);
            return 1;
        }
    }

    sqlite3_finalize(stmt);
    return 0;
}
int 
SQLiteWrapper::is_user_friend_exists(const std::string user_account, const std::string friend_account)
{
    char sql[256] = {0};
    sqlite3_stmt *stmt = nullptr;
    const char *ztail = nullptr;
    sprintf(sql, "SELECT user_account,friend_account FROM user_friend_table WHERE user_account = '%s'", user_account.c_str());
    int ret = sqlite3_prepare_v2(user_database, sql, -1, &stmt, &ztail);
    if (ret != SQLITE_OK) {
        LOG_ERROR("SQLiteWrapper:: select_users_friends calls failed: %s", sqlite3_errmsg(user_database));
        return -1;
    }

    int pos = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        if (strncmp(friend_account.c_str(), (const char *)sqlite3_column_text(stmt, 1), strlen(friend_account.c_str())) == 0) {
            sqlite3_finalize(stmt);
            return 1;
        }
    }

    sqlite3_finalize(stmt);
    return 0;
}
int 
SQLiteWrapper::is_group_exists(const std::string group_account)
{
    char sql[256] = {0};
    sqlite3_stmt *stmt = nullptr;
    const char *ztail = nullptr;
    sprintf(sql, "SELECT group_account,manager_account FROM group_table WHERE group_account = '%s'", group_account.c_str());
    int ret = sqlite3_prepare_v2(user_database, sql, -1, &stmt, &ztail);
    if (ret != SQLITE_OK) {
        LOG_ERROR("SQLiteWrapper:: select_group_from_group_table calls failed: %s", sqlite3_errmsg(user_database));
        return -1;
    }

    int pos = 0;
    while(sqlite3_step(stmt) == SQLITE_ROW) {
        if (strncmp(group_account.c_str(), (const char *)sqlite3_column_text(stmt, 0), strlen(group_account.c_str())) == 0) {
            sqlite3_finalize(stmt);
            return 1;
        }
    }

    sqlite3_finalize(stmt);
    return 0;
}
int 
SQLiteWrapper::is_group_member_exists(const std::string group_account, const std::string member_account)
{
    char sql[256] = {0};
    sqlite3_stmt *stmt = nullptr;
    const char *ztail = nullptr;
    sprintf(sql, "SELECT group_account, member_account FROM group_member_table WHERE group_account = '%s' AND member_account = '%s'", group_account.c_str(), member_account.c_str());
    int ret = sqlite3_prepare_v2(user_database, sql, -1, &stmt, &ztail);
    if (ret != SQLITE_OK) {
        LOG_ERROR("SQLiteWrapper:: select_groups_members calls failed: %s", sqlite3_errmsg(user_database));
        return -1;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        if (strncmp(member_account.c_str(), (const char *)sqlite3_column_text(stmt, 1), strlen(member_account.c_str())) == 0) {
            sqlite3_finalize(stmt);
            return 1;
        }
    }

    sqlite3_finalize(stmt);
    return 0;
}
int 
SQLiteWrapper::check_user_passwd(const std::string user_account, const std::string passwd)
{
    char sql[256] = {0};
    sqlite3_stmt *stmt = nullptr;
    const char *ztail = nullptr;
    sprintf(sql, "SELECT user_password FROM user_table WHERE user_account = '%s'", user_account.c_str());
    int ret = sqlite3_prepare_v2(user_database, sql, -1, &stmt, &ztail);
    if (ret != SQLITE_OK) {
        LOG_ERROR("SQLiteWrapper:: select_user_from_user_table calls failed: %s", sqlite3_errmsg(user_database));
        return -1;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* password_str = (const char *)sqlite3_column_text(stmt, 0);
        if (password_str == nullptr) {
            break;
        }
        std::string password(password_str);
        if (password.compare(passwd) == 0) {
            sqlite3_finalize(stmt);
            return 1;
        }
    }

    sqlite3_finalize(stmt);
    return 0;
}


int
SQLiteWrapper::init_database_tables(void)
{
    std::string sqls = "CREATE TABLE user_table("  \
         "user_account      CHAR(50)    NOT NULL," \
         "user_name         CHAR(50)    NOT NULL," \
         "user_password     CHAR(50)    NOT NULL);";
    int ret = sqlite3_exec(user_database, sqls.c_str(), NULL, 0, &err_msg);
    if (ret != SQLITE_OK) {
        LOG_ERROR("SQLiteWrapper:: init_user_table failed: %s", err_msg);
        sqlite3_free(err_msg);
        exit(EXIT_FAILURE);
    } else {
        LOG_INFO("SQLiteWrapper:: init_user_tables successful.");
    }

    sqls = "CREATE TABLE user_friend_table("  \
         "user_account      CHAR(50)    NOT NULL," \
         "friend_account    CHAR(50)    NOT NULL);";
    ret = sqlite3_exec(user_database, sqls.c_str(), NULL, 0, &err_msg);
    if (ret != SQLITE_OK) {
        LOG_ERROR("SQLiteWrapper:: init_user_friend_tables failed: %s", err_msg);
        sqlite3_free(err_msg);
        exit(EXIT_FAILURE);
    } else {
        LOG_INFO("SQLiteWrapper:: init_user_friend_tables successful.");
    }

    sqls = "CREATE TABLE group_table("  \
         "group_account      CHAR(50)    NOT NULL," \
         "group_name         CHAR(50)    NOT NULL," \
         "manager_account    CHAR(50)    NOT NULL);";
    ret = sqlite3_exec(user_database, sqls.c_str(), NULL, 0, &err_msg);
    if (ret != SQLITE_OK) {
        LOG_ERROR("SQLiteWrapper:: init_group_tables failed: %s", err_msg);
        sqlite3_free(err_msg);
        exit(EXIT_FAILURE);
    } else {
        LOG_INFO("SQLiteWrapper:: init_group_tables successful.");
    }

    sqls = "CREATE TABLE group_member_table("  \
         "group_account      CHAR(50)    NOT NULL," \
         "member_account     CHAR(50)    NOT NULL);";
    ret = sqlite3_exec(user_database, sqls.c_str(), NULL, 0, &err_msg);
    if (ret != SQLITE_OK) {
        LOG_ERROR("SQLiteWrapper:: init_group_member_tables failed: %s", err_msg);
        sqlite3_free(err_msg);
        exit(EXIT_FAILURE);
    } else {
        LOG_INFO("SQLiteWrapper:: init_group_member_tables successful.");
    }

    sqls = "CREATE TABLE user_managered_group_table("\
            "manager_account            CHAR(50)    NOT NULL,"\
            "managered_group_account    CHAR(50)    NOT NULL);";
    ret = sqlite3_exec(user_database, sqls.c_str(), NULL, 0, &err_msg);
    if (ret != SQLITE_OK) {
        LOG_ERROR("SQLiteWrapper:: init_manager_group_tables failed: %s", err_msg);
        sqlite3_free(err_msg);
        exit(EXIT_FAILURE);
    } else {
        LOG_INFO("SQLiteWrapper:: init_manager_group_tables successful.");
    }

    sqls = "CREATE TABLE user_joined_group_table("\
            "user_account           CHAR(50)    NOT NULL,"\
            "joined_group_account   CHAR(50) NOT NULL);";
    ret = sqlite3_exec(user_database, sqls.c_str(), NULL, 0, &err_msg);
    if (ret != SQLITE_OK) {
        LOG_ERROR("SQLiteWrapper:: init_user_group_tables failed: %s", err_msg);
        sqlite3_free(err_msg);
        exit(EXIT_FAILURE);
    } else {
        LOG_INFO("SQLiteWrapper:: init_user_group_tables successful.");
    }
}

int
SQLiteWrapper::insert_user_record_to_user_table(struct database_table &table)
{
    if (table.table != USER_TABLE) {
        LOG_WARN("SQLiteWrapper:: insert_user_record_to_user_table: trying to insert a wrong record");
        return -1;
    }

    char sql[128] = {0};
    sprintf(sql, "INSERT INTO user_table(user_account, user_name, user_password) " \
            "VALUES('%s', '%s', '%s');", table.user.user_account.c_str(), table.user.user_name.c_str(), table.user.user_password.c_str());
    int ret = sqlite3_exec(user_database, sql, NULL, 0, &err_msg);
    if (ret != SQLITE_OK) {
        LOG_ERROR("SQLiteWrapper:: insert_user_record_to_user_table sqlite3_exec failed: %s", err_msg);
        sqlite3_free(err_msg);
        return -1;
    } else {
        LOG_INFO("Successfully create a user record: %s", table.user.user_account.c_str());
    }
    return 0;
}

int
SQLiteWrapper::insert_user_friend_record_to_user_friend_table(struct database_table &table)
{
    if (table.table != USER_FRIEND_TABLE) {
        LOG_WARN("SQLiteWrapper:: insert_user_friend_record_to_user_friend_table: trying to insert a wrong record");
        return -1;
    }

    char sql[128] = {0};
    sprintf(sql, "INSERT INTO user_friend_table(user_account, friend_account)" \
            "VALUES('%s', '%s');", table.user_friend.user_account.c_str(), table.user_friend.friend_account.c_str());
    int ret = sqlite3_exec(user_database, sql, NULL, 0, &err_msg);
    if (ret != SQLITE_OK) {
        LOG_ERROR("SQLiteWrapper:: insert_user_friend_record_to_user_friend_table sqlite3_exec failed: %s", err_msg);
        sqlite3_free(err_msg);
        return -1;
    } else {
        LOG_INFO("Successfully create a user_friend record: user: %s friend: %s", table.user_friend.user_account.c_str(), table.user_friend.friend_account.c_str());
    }
    return 0;
}

int
SQLiteWrapper::insert_group_record_to_group_table(struct database_table &table)
{
    if (table.table != GROUP_TABLE) {
        LOG_WARN("SQLiteWrapper:: insert_group_record_to_group_table: trying to insert a wrong record");
        return -1;
    }

    char sql[128] = {0};
    sprintf(sql, "INSERT INTO group_table(group_account, group_name ,manager_account)" \
            "VALUES('%s', '%s', '%s');", table.group.group_account.c_str(), table.group.group_name.c_str(), table.group.group_manager_account.c_str());
    int ret = sqlite3_exec(user_database, sql, NULL, 0, &err_msg);
    if (ret != SQLITE_OK) {
        LOG_ERROR("SQLiteWrapper:: insert_group_record_to_group_table sqlite3_exec failed: %s", err_msg);
        sqlite3_free(err_msg);
        return -1;
    } else {
        LOG_INFO("Successfully create a group record: %s", table.group.group_account.c_str());
    }
    return 0;
}

int
SQLiteWrapper::insert_group_member_record_to_group_member_table(struct database_table &table)
{
    if (table.table != GROUP_MEMBER_TABLE) {
        LOG_WARN("SQLiteWrapper:: insert_group_member_record_to_group_member_table: trying to insert a wrong record");
        return -1;
    }

    char sql[128] = {0};
    sprintf(sql, "INSERT INTO group_member_table(group_account, member_account)" \
            "VALUES('%s', '%s');", table.group_member.group_account.c_str(), table.group_member.group_member_account.c_str());
    int ret = sqlite3_exec(user_database, sql, NULL, 0, &err_msg);
    if (ret != SQLITE_OK) {
        LOG_ERROR("SQLiteWrapper:: insert_group_member_record_to_group_member_table sqlite3_exec failed: %s", err_msg);
        sqlite3_free(err_msg);
        return -1;
    } else {
        LOG_INFO("Successfully create a group member record: group: %s member: %s", table.group_member.group_account.c_str(), table.group_member.group_member_account.c_str());
    }
    return 0;
}

int
SQLiteWrapper::insert_user_managered_group_record_to_user_managered_group_table(struct database_table &table)
{
    if (table.table != USER_MANAGERED_GROUP_TABLE) {
        LOG_WARN("SQLIteWrapper:: insert_user_managered_group_record_to_user_managered_group_table: trying to insert a wrong record");
        return -1;
    }

    char sql[128] = {0};
    sprintf(sql, "INSERT INTO user_managered_group_table(manager_account, managered_group_account)" \
                "VALUES('%s','%s');", table.user_managered_group.manager_account.c_str(), table.user_managered_group.managered_group_account.c_str());
    int ret = sqlite3_exec(user_database, sql, NULL, 0, &err_msg);
    if (ret != SQLITE_OK) {
        LOG_ERROR("SQLiteWrapper:: insert_user_managered_group_record_to_user_managered_group_table sqlite3_exec failed: %s", err_msg);
        sqlite3_free(err_msg);
        return -1;
    } else {
        LOG_INFO("Successfully create a manager group record: manager: %s group: %s", table.user_managered_group.manager_account.c_str(), table.user_managered_group.managered_group_account.c_str());
    }
    return 0;
}

int
SQLiteWrapper::insert_user_joined_group_record_to_user_joined_group_table(struct database_table &table)
{
    if (table.table != USER_JOINED_GROUP_TABLE) {
        LOG_WARN("SQLiteWrapper:: insert_user_joined_group_record_to_user_joined_group_table: trying to insert a wrong record");
        return -1;
    }

    char sql[128] = {0};
    sprintf(sql, "INSERT INTO user_joined_group_table(user_account, joined_group_account)" \
                "VALUES('%s','%s');", table.user_joined_group.user_account.c_str(), table.user_joined_group.joined_group_account.c_str());
    int ret = sqlite3_exec(user_database, sql, NULL, 0, &err_msg);
    if (ret != SQLITE_OK) {
        LOG_ERROR("SQLiteWrapper:: insert_user_joined_group_record_to_user_joined_group_table sqlite3_exec failed: %s", err_msg);
        sqlite3_free(err_msg);
        return -1;
    } else {
        LOG_INFO("Successfully create a user group record: manager: %s group: %s", table.user_joined_group.user_account.c_str(), table.user_joined_group.joined_group_account.c_str());
    }
    return 0;
}

int
SQLiteWrapper::select_user_from_user_table(std::string user_account, struct database_table &table)
{
    char sql[256] = {0};
    sqlite3_stmt *stmt = nullptr;
    const char *ztail = nullptr;

    sprintf(sql, "SELECT user_account, user_name ,user_password FROM user_table WHERE user_account = '%s';", user_account.c_str());
    int ret = sqlite3_prepare_v2(user_database, sql, strlen(sql), &stmt, &ztail);
    if (ret != SQLITE_OK) {
        LOG_ERROR("SQLiteWrapper:: select_user_from_user_table calls failed: %s", sqlite3_errmsg(user_database));
        return -1;
    }
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char *tmp_account = (const char *)sqlite3_column_text(stmt, 0);
        const char *tmp_name = (const char *)sqlite3_column_text(stmt, 1);
        const char *tmp_password = (const char *)sqlite3_column_text(stmt, 2);

        if (tmp_account == nullptr || tmp_name == nullptr || tmp_password == nullptr) {
            continue;
        }
        table.user.user_account = tmp_account; 
        table.user.user_name = tmp_name;
        table.user.user_password = tmp_password;
    }

    sqlite3_finalize(stmt);
    return 0;
}

int 
SQLiteWrapper::select_all_users_from_user_table(std::vector<struct database_table> &users)
{
    char sql[256] = {0};
    sqlite3_stmt *stmt = nullptr;
    const char *ztail = nullptr;
    sprintf(sql, "SELECT user_account, user_name ,user_password FROM user_table;");
    int ret = sqlite3_prepare_v2(user_database, sql, strlen(sql), &stmt, &ztail);
    if (ret != SQLITE_OK) {
        LOG_ERROR("SQLiteWrapper:: select_all_users_from_user_table calls failed: %s", sqlite3_errmsg(user_database));
        return -1;
    }

    struct database_table tmp;
    users.clear();
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char *tmp_account = (const char *)sqlite3_column_text(stmt, 0);
        const char *tmp_name = (const char *)sqlite3_column_text(stmt, 1);
        const char *tmp_password = (const char *)sqlite3_column_text(stmt, 2);

        if (tmp_account == nullptr || tmp_name == nullptr || tmp_password == nullptr) {
            continue;
        }
        tmp.user.user_account = tmp_account; 
        tmp.user.user_name = tmp_name;
        tmp.user.user_password = tmp_password;
        users.push_back(tmp);
    }

    sqlite3_finalize(stmt);
    return 0;
}

int
SQLiteWrapper::select_users_friends(const std::string user_account, std::vector<std::string> &friends)
{
    char sql[256] = {0};
    sqlite3_stmt *stmt = nullptr;
    const char *ztail = nullptr;
    sprintf(sql, "SELECT user_account, friend_account FROM user_friend_table WHERE user_account = '%s';", user_account.c_str());
    int ret = sqlite3_prepare_v2(user_database, sql, -1, &stmt, &ztail);
    if (ret != SQLITE_OK) {
        LOG_ERROR("SQLiteWrapper:: select_users_friends calls failed: %s", sqlite3_errmsg(user_database));
        return -1;
    }

    friends.clear();
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char *tmp_friend_acount = (const char *)sqlite3_column_text(stmt, 1);
        if (tmp_friend_acount == nullptr) {
            continue;
        }
        friends.push_back(tmp_friend_acount);
    }

    sqlite3_finalize(stmt);
    return 0;
}

int 
SQLiteWrapper::select_group_from_group_table(const std::string group_account, struct database_table &table)
{
    std::vector<struct database_table> tmp_groups;
    if (select_all_groups_from_group_table(tmp_groups) == -1) {
        LOG_ERROR("SQLiteWrapper:: select_group_from_group_table calls failed.");
        return -1;
    }

    for (auto &tmp : tmp_groups) {
        if (tmp.group.group_account.compare(group_account) == 0) {
            table = tmp;
            return 0;
        }
    }
    LOG_WARN("select_group_from_group_table calls failed: No such group_account: %s.", group_account.c_str());
    return -1;
}

int 
SQLiteWrapper::select_all_groups_from_group_table(std::vector<struct database_table> &groups)
{
    char sql[256] = {0};
    sqlite3_stmt *stmt = nullptr;
    const char *ztail = nullptr;
    sprintf(sql, "SELECT group_account, group_name, manager_account FROM group_table;");
    int ret = sqlite3_prepare_v2(user_database, sql, -1, &stmt, &ztail);
    if (ret != SQLITE_OK) {
        LOG_ERROR("SQLiteWrapper:: select_all_groups_from_group_table calls failed: %s", sqlite3_errmsg(user_database));
        return -1;
    }

    struct database_table tmp;
    groups.clear();
    while(sqlite3_step(stmt) == SQLITE_ROW) {
        const char *tmp_account = (const char *)sqlite3_column_text(stmt, 0);
        const char *tmp_name = (const char *)sqlite3_column_text(stmt, 1);
        const char *tmp_manager_account = (const char *)sqlite3_column_text(stmt, 2);
        if (tmp_account == nullptr || tmp_name == nullptr || tmp_manager_account == nullptr) {
            continue;
        }
        tmp.group.group_account = tmp_account;
        tmp.group.group_name = tmp_name;
        tmp.group.group_manager_account = tmp_manager_account;
        groups.push_back(tmp);
    }

    sqlite3_finalize(stmt);
    return 0;
}

int
SQLiteWrapper::select_groups_members(const std::string group_account, std::vector<std::string> &members)
{
    char sql[256] = {0};
    sqlite3_stmt *stmt = nullptr;
    const char *ztail = nullptr;
    sprintf(sql, "SELECT group_account, member_account FROM group_member_table WHERE group_account = '%s';", group_account.c_str());
    int ret = sqlite3_prepare_v2(user_database, sql, -1, &stmt, &ztail);
    if (ret != SQLITE_OK) {
        LOG_ERROR("SQLiteWrapper:: select_groups_members calls failed: %s", sqlite3_errmsg(user_database));
        return -1;
    }

    int pos = 0;
    members.clear();
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* tmp_group_account = (const char *)sqlite3_column_text(stmt, 1);
        if (tmp_group_account == nullptr) {
            continue;
        }
        members.push_back(tmp_group_account);
    }

    sqlite3_finalize(stmt);
    return pos;
}

int
SQLiteWrapper::select_all_user_managered_groups(const std::string manager_account, std::vector<std::string> &groups)
{
    char sql[256] = {0};
    sqlite3_stmt *stmt = nullptr;
    const char *ztail = nullptr;
    sprintf(sql, "SELECT managered_group_account FROM user_managered_group_table WHERE manager_account = '%s';", manager_account.c_str());
    int ret = sqlite3_prepare_v2(user_database, sql, -1, &stmt, &ztail);
    if (ret != SQLITE_OK) {
        LOG_ERROR("SQLiteWrapper:: select_all_user_managered_groups calls failed: %s", sqlite3_errmsg(user_database));
        return -1;
    }

    int pos = 0;
    groups.clear();
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char *tmp_group_account = (const char *)sqlite3_column_text(stmt, 0);
        if (tmp_group_account == nullptr) {
            continue;
        }
        groups.push_back(tmp_group_account);
    }

    sqlite3_finalize(stmt);
    return pos;
}

int
SQLiteWrapper::select_all_user_jonined_groups(const std::string user_account, std::vector<std::string> &groups)
{
    char sql[256] = {0};
    sqlite3_stmt *stmt = nullptr;
    const char *ztail = nullptr;
    sprintf(sql, "SELECT joined_group_account FROM user_joined_group_table WHERE user_account = '%s';", user_account.c_str());
    int ret = sqlite3_prepare_v2(user_database, sql, -1, &stmt, &ztail);
    if (ret != SQLITE_OK) {
        LOG_ERROR("SQLiteWrapper:: select_all_user_jonined_groups calls failed: %s", sqlite3_errmsg(user_database));
        return -1;
    }

    int pos = 0;
    groups.clear();
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char *tmp_group_account = (const char *)sqlite3_column_text(stmt, 0);
        if (tmp_group_account == nullptr) {
            continue;
        }
        groups.push_back(tmp_group_account);
    }

    sqlite3_finalize(stmt);
    return pos;
}

int 
SQLiteWrapper::update_user_name_table(struct database_table &table)
{
    char sql[256] = {0};
    sprintf(sql, "UPDATE user_table SET user_name = '%s' WHERE user_account = '%s';", table.user.user_name.c_str(), table.user.user_account.c_str());
    
    int ret = sqlite3_exec(user_database, sql, nullptr, 0 , &err_msg);
    if (ret != SQLITE_OK) {
        LOG_ERROR("SQLiteWrapper:: update_user_table calls failed: %s", err_msg);
        sqlite3_free(err_msg);
        return -1;
    }
    return 0;
}

int 
SQLiteWrapper::update_user_passwd_table(struct database_table &table)
{
    char sql[256] = {0};
    sprintf(sql, "UPDATE user_table SET user_password = '%s' WHERE user_account = '%s';", table.user.user_password.c_str(), table.user.user_account.c_str());
    
    int ret = sqlite3_exec(user_database, sql, nullptr, 0 , &err_msg);
    if (ret != SQLITE_OK) {
        LOG_ERROR("SQLiteWrapper:: update_user_passwd_table calls failed: %s", err_msg);
        sqlite3_free(err_msg);
        return -1;
    }
    return 0;
}

int 
SQLiteWrapper::update_group_manager_table(struct database_table &table)
{
    char sql[256] = {0};
    sprintf(sql, "UPDATE group_table SET manager_account = '%s' WHERE group_account = '%s';", table.group.group_manager_account.c_str(), table.group.group_account.c_str());
    
    int ret = sqlite3_exec(user_database, sql, nullptr, 0 , &err_msg);
    if (ret != SQLITE_OK) {
        LOG_ERROR("SQLiteWrapper:: update_group_manager_table calls failed: %s", err_msg);
        sqlite3_free(err_msg);
        return -1;
    }
    return 0;
}
int 
SQLiteWrapper::update_group_name_table(struct database_table &table)
{
    char sql[256] = {0};
    sprintf(sql, "UPDATE group_table SET group_name = '%s' WHERE group_account = '%s';", table.group.group_name.c_str(), table.group.group_account.c_str());
    
    int ret = sqlite3_exec(user_database, sql, nullptr, 0 , &err_msg);
    if (ret != SQLITE_OK) {
        LOG_ERROR("SQLiteWrapper:: update_group_name_table calls failed: %s", err_msg);
        sqlite3_free(err_msg);
        return -1;
    }
    return 0;
}

int 
SQLiteWrapper::delete_friend_from_user_friend_table(const std::string user_account, const std::string friend_account)
{
    char sql[256] = {0};
    sprintf(sql, "DELETE FROM user_friend_table WHERE user_account = '%s' AND friend_account = '%s';", user_account.c_str(), friend_account.c_str());
    
    int ret = sqlite3_exec(user_database, sql, nullptr, 0 , &err_msg);
    if (ret != SQLITE_OK) {
        LOG_ERROR("SQLiteWrapper:: delete_friend_from_user_friend_table calls failed: %s", err_msg);
        sqlite3_free(err_msg);
        return -1;
    }
    return 0;
}

int 
SQLiteWrapper::delete_member_from_group_member_table(const std::string group_account, const std::string member_account)
{
    char sql[256] = {0};
    sprintf(sql, "DELETE FROM group_member_table WHERE group_account = '%s' AND member_account = '%s';", group_account.c_str(), member_account.c_str());
    
    int ret = sqlite3_exec(user_database, sql, nullptr, 0 , &err_msg);
    if (ret != SQLITE_OK) {
        LOG_ERROR("SQLiteWrapper:: delete_member_from_group_member_table calls failed: %s", err_msg);
        sqlite3_free(err_msg);
        return -1;
    }
    return 0;
}

int
SQLiteWrapper::delete_group_from_group_table(const std::string group_account)
{
    char sql[256] = {0};
    sprintf(sql, "DELETE FROM group_table WHERE group_account = '%s';", group_account.c_str());

    int ret = sqlite3_exec(user_database, sql, nullptr, 0, &err_msg);
    if (ret != SQLITE_OK) {
        LOG_ERROR("SQLiteWrapper:: delete_group_from_group_table calls failed: %s", err_msg);
        sqlite3_free(err_msg);
        return -1;
    }
    return 0;
}

int
SQLiteWrapper::delete_group_from_user_managered_group_table(const std::string manager_account, const std::string group_account)
{
    if (delete_group_from_group_table(group_account) == -1) {
        LOG_WARN("SQLiteWrapper:: delete_group_from_user_managered_group_table(): delete_group_from_group_table.");
        return -1;
    }

    std::vector<std::string> tmp;
    if (select_groups_members(group_account, tmp) == -1) {
        LOG_WARN("SQLiteWrapper:: delete_group_from_user_managered_group_table(): select_groups_member calls failed.");
        return -1;
    }

    for (auto tmp_user : tmp) {
        if (delete_group_from_user_joined_group_table(tmp_user, group_account) == -1) {
            LOG_WARN("SQLiteWrapper:: delete_group_from_user_managered_group_table(): delete_group_from_user_joined_group_table(%s, %s) calls failed.", tmp_user.c_str(), group_account.c_str());
            continue;
        }
    }
}

int
SQLiteWrapper::delete_group_from_user_joined_group_table(const std::string user_account, const std::string group_account)
{
    char sql[256] = {0};
    sprintf(sql, "DELETE FROM group_member_table WHERE group_account = '%s' AND member_account = '%s';", group_account.c_str(), user_account.c_str());

    int ret = sqlite3_exec(user_database, sql, nullptr, 0, &err_msg);
    if (ret != SQLITE_OK) {
        LOG_ERROR("SQLiteWrapper:: delete_group_from_user_joined_group_table calls failed: %s", err_msg);
        sqlite3_free(err_msg);
        return -1;
    }
    return 0;
}