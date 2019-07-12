#include "server_json.h"
#include "server_log.h"
#include "queue.h"
#include "msg_buffer.h"
#include "sqlite_wrapper.h"
#include "user_manager.h"
#include "msg_fragment.h"
#include "fragment_operate.h"

int main(void)
{
    Json& json_instance = Json::get_json_instance("../../server_config/server.json");
    SQLiteWrapper& sqlite_instance = SQLiteWrapper::get_sqlitewrapper_instance(json_instance);
    UserManager& user_manager_instance = UserManager::get_user_manager_instance(sqlite_instance, json_instance);

    FRAGMENT_SEND_RECV_QUEUE_INIT(json_instance);
    MSG_BUFFER_INIT();

    std::vector<struct database_table> users;
    sqlite_instance.select_all_users_from_user_table(users);

    struct fragment tmp_frag;

    for (auto tmp : users) {
        FragmentOperate::set_sender_account(tmp_frag, tmp.user.user_account);
        FRAGMENT_SEND_PUSH(tmp_frag);
        std::cout << "send push account: " << tmp.user.user_account << std::endl;
    }
    std::cout << "send queue current size: " << FRAGMENT_SEND_SIZE() << std::endl << std::endl;

    struct fragment pop_frag;
    while (FRAGMENT_SEND_POP(pop_frag) != -1) {
        std::cout << "send pop account: " << pop_frag.sender_account << std::endl;
        FRAGMENT_RECV_PUSH(pop_frag);
        std::cout << "recv push account: " << pop_frag.sender_account << std::endl;
    }
    std::cout << "send queue current size: " << FRAGMENT_SEND_SIZE() << std::endl;
    std::cout << "recv queue current size: " << FRAGMENT_RECV_SIZE() << std::endl;

    std::cout << std::endl;
    while (FRAGMENT_RECV_POP(pop_frag) != -1) {
        std::cout << pop_frag.sender_account << std::endl;
        std::cout << "recv pop account: " << pop_frag.sender_account << std::endl;
    }
    std::cout << "recv queue current size: " << FRAGMENT_RECV_SIZE() << std::endl << std::endl;;

    for (int i = 1; i <= 5; ++i) {
        for (int j = i+5; j < i+12; ++j) {
            FragmentOperate::set_sender_account(tmp_frag, users[j].user.user_account);
            FragmentOperate::set_receiver_account(tmp_frag, users[i].user.user_account);
            MSG_BUFFER_PUSH(tmp_frag);
        }
    }

    std::vector<struct fragment> msgs;
    for (int i = 1; i <= 5; ++i) {
        MSG_BUFFER_POP(users[i].user.user_account, msgs);
        std::cout << "user: " << users[i].user.user_account << " unreceived message." << std::endl;
        for (int i = 0; i < msgs.size(); ++i) {
            std::cout << msgs[i].sender_account << std::endl;
        }
        std::cout << std::endl;
    }
    std::cout << "search again" << std::endl;
    for (int i = 1; i <= 5; ++i) {
        MSG_BUFFER_POP(users[i].user.user_account, msgs);
        std::cout << "user: " << users[i].user.user_account << " unreceived message." << std::endl;
        for (int i = 0; i < msgs.size(); ++i) {
            std::cout << msgs[i].sender_account << std::endl;
        }
        std::cout << std::endl;
    }
    return 0;
}