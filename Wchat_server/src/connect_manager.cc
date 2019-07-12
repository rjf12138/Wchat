#include "connect_manager.h"

ConnectManager&
ConnectManager::get_connect_manager_instance(void)
{
    static ConnectManager d_connect_manager;
    return d_connect_manager;
}

int
ConnectManager::add_user_connect(const std::string& user_account, struct bufferevent* buf)
{
    d_user_connect[user_account] = {CONNECT_LIVING_TIME, buf};
    return 0;
}

struct bufferevent*
ConnectManager::get_user_connect(const std::string& user_account)
{
    auto connect_iter = d_user_connect.find(user_account);
    if (connect_iter == d_user_connect.end()) {
        return nullptr;
    }

    return d_user_connect[user_account].d_buf;
}

int
ConnectManager::remove_user_connect(const std::string& user_account)
{
    auto connect_iter = d_user_connect.find(user_account);
    if (connect_iter == d_user_connect.end()) {
        return -1;
    }

    d_user_connect.erase(connect_iter);
    return 0;
}

int
ConnectManager::add_unknown_connect(struct fragment& frag, struct bufferevent* buf)
{
    frag.tmp = d_unknown_connect_num;
    d_unknown_connect[d_unknown_connect_num].d_buf = buf;
    d_unknown_connect_num += 1;

    return 0;
}

struct bufferevent*
ConnectManager::get_unknown_connect(uint unknown_connect_num)
{
    auto connect_iter = d_unknown_connect.find(unknown_connect_num);
    if (connect_iter == d_unknown_connect.end()) {
        return nullptr;
    }

    return d_unknown_connect[unknown_connect_num].d_buf;
}

int
ConnectManager::remove_unknown_connect(uint unknown_connect_num)
{
    auto connect_iter = d_unknown_connect.find(unknown_connect_num);
    if (connect_iter == d_unknown_connect.end()) {
        return -1;
    }

    d_unknown_connect.erase(connect_iter);
    return 0;
}

bool
ConnectManager::check_msg_is_send_from_unknown_user(struct fragment& fragment)
{
   if (fragment.msg_type == SIGN_UP || fragment.msg_type == SIGN_IN) {
      return true;
   }

   return false;
}

int 
ConnectManager::remove_timeout_user_connect(void)
{
    for (auto t_user_iter : d_user_connect) {
        if (t_user_iter.second.d_live_time - ALARM_TIME < 0) {
            struct fragment t_notify_server_user_offline;
            FragmentOperate::set_msg_type(t_notify_server_user_offline, SIGN_OUT);                  // 以下几行是模拟消息通知服务器用户因超时而被强制下线了
            FragmentOperate::set_sender_account(t_notify_server_user_offline, t_user_iter.first);
            remove_user_connect(t_user_iter.first);
            FRAGMENT_RECV_PUSH(t_notify_server_user_offline);
        } else {
            t_user_iter.second.d_live_time -= ALARM_TIME;
        }
    }
}

int 
ConnectManager::remove_timeout_unknown_connect(void)
{
    for (auto t_unknown_iter : d_unknown_connect) {
        if (t_unknown_iter.second.d_live_time - ALARM_TIME < 0) {
            remove_unknown_connect(t_unknown_iter.first);
        } else {
            t_unknown_iter.second.d_live_time -= ALARM_TIME;
        }
    }
}

bool 
ConnectManager::is_signal_trigger(void)
{
    return d_signal_trigger;
}

void
ConnectManager::set_signal_trigger(bool p_signal_trigger)
{
    d_signal_trigger = p_signal_trigger;
}