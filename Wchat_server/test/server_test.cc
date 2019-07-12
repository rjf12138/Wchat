#include "server_log.h"
#include "server_json.h"
#include "account_manager.h"

int main()
{
    Json& json_instance = Json::get_json_instance("../../server_config/server.json");
    AccountManager &am = AccountManager::get_account_manager_instance(json_instance);
    int ser_port, start_account;
    std::string server_addr;

    json_instance.read_value("server-addr",server_addr);
    json_instance.read_value("server-port",ser_port);
    std::cout << "user_account: " << am.next_user_account();
    std::cout << "group_account: " << am.next_group_account();
    fprintf(stderr,"%s\n",server_addr.c_str());
    fprintf(stderr,"%d\n",ser_port);
    LOG_ERROR("log_error___");
    LOG_INFO("log_info___");
    LOG_INFO("server-addr: %s",server_addr.c_str());
    LOG_INFO("ser_port: %d", ser_port);
    LOG_CONSOLE("print to console");

    return 0;
}