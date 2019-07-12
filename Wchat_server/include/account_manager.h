#ifndef __ACCOUNT_MANAGER_H__
#define __ACCOUNT_MANAGER_H__

#include <string>
#include <fstream>

#include "server_json.h"
#include "server_log.h"

#define START_USER_ACCOUNT  "start_user_account"
#define START_GROUP_ACCOUNT "start_group_account"
#define ACCOUNT_JSON_PATH   "account_json"

class AccountManager {
public:
    static AccountManager& get_account_manager_instance(Json &json_instance);   // 返回一个 AccountManager 实例
    std::string next_user_account(void);                                        // 返回一个新的用户账号
    std::string next_group_account(void);                                       // 返回一个新的群账号

private:
    AccountManager(std::string json_file_name);
    ~AccountManager();
private:
    nlohmann::json  account_js;                                                 // 保存下个新用户账号和新群账号的 json
    std::fstream    json_file;                                                  // 打开 json 文件流
    std::string account_json_path;                                              // account.json 文件的路径
};

#endif
