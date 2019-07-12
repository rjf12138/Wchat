#include <cstring>

#include "account_manager.h"

AccountManager&
AccountManager::get_account_manager_instance(Json &json_instance)
{
    std::string account_json_file_path;
    json_instance.read_value(ACCOUNT_JSON_PATH, account_json_file_path);
    static AccountManager account_manager_instance(account_json_file_path); // 读取 account.json 的路径
    return account_manager_instance;
}

AccountManager::AccountManager(std::string json_file_name)
{
    account_json_path = json_file_name;
    json_file.open(account_json_path, std::fstream::in);   // 以只读的方式打开
    if (json_file.good()) {
        if (json_file.eof()) {
            LOG_ERROR("AccountManager::AccountManager calls failed: account.json is empty.");
            exit(EXIT_FAILURE);
        }
        json_file >> account_js;
    } else {
        LOG_ERROR("AccountManager::AccountManager calls failed: open %s failed.", account_json_path.c_str());
        exit(EXIT_FAILURE);
    }

    json_file.close();
}
std::string
AccountManager::next_user_account(void)
{
    char account[32];
    memset(account, '\0', 32);
    int account_value = account_js[START_USER_ACCOUNT];
    snprintf(account, 32,"%d", account_value);
    account_js[START_USER_ACCOUNT] = account_value + 1;
    return std::string(account);
}
std::string
AccountManager::next_group_account(void)
{
    char account[32];
    memset(account, '\0', 32);
    int account_value = account_js[START_GROUP_ACCOUNT];
    snprintf(account, 32,"%d", account_value);
    account_js[START_GROUP_ACCOUNT] = account_value + 1;
    return std::string(account);
}

AccountManager::~AccountManager()
{
    json_file.open(account_json_path, std::fstream::out);  // 以写的方式打开
    if (!json_file.good()) {
        LOG_ERROR("AccountManager::~AccountManager calls failed: open %s failed.", account_json_path.c_str());
        exit(EXIT_FAILURE);
    }
    json_file << account_js;
    json_file.close();
}
