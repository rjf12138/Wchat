#ifndef __CONNECT_MANAGER_H__
#define __CONNECT_MANAGER_H__

#include <map>
#include <event2/event.h>
#include <event2/bufferevent.h>

#include "msg_fragment.h"
#include "fragment_operate.h"
#include "msg_type.h"

#define CONNECT_LIVING_TIME              200         // (单位：秒)链接的默认存活时间： 未知链接超过这个时间就会被清除， 用户链接则会进行心跳验证如果验证失败则删除链接
#define ALARM_TIME                       20          // (单位：秒)每隔这段时间会去确认所有链接

struct user_connect {
    int d_live_time;
    struct bufferevent* d_buf;
};

class ConnectManager {
public:
    static ConnectManager& get_connect_manager_instance(void);                          // 返回一个 ConnectManager 的实例

    int add_user_connect(const std::string& user_account, struct bufferevent* buf);     // 保存一个用户-链接对
    struct bufferevent* get_user_connect(const std::string& user_account);              // 根据用户账号返回一个链接
    int remove_user_connect(const std::string& user_account);                           // 移除一个用户链接

    int add_unknown_connect(struct fragment& frag, struct bufferevent* buf);            // 添加一个未知的链接并分配一个链接编号
    struct bufferevent* get_unknown_connect(uint connect_num);                          // 根据链接编号返回一个用户链接
    int remove_unknown_connect(uint connect_num);                                       // 移除一个未知的用户链接

    bool check_msg_is_send_from_unknown_user(struct fragment& fragment);                // 根据 msg_type 判断消息是不是发自未知用户
    int remove_timeout_user_connect(void);                                              // 移除超时的用户链接
    int remove_timeout_unknown_connect(void);                                           // 移除超时的未知用户链接
    bool is_signal_trigger(void);                                                       // 返回signal_trigger 当前的状态
    void set_signal_trigger(bool p_signal_trigger);                                     // 设置 signal_trigger 当前的状态。true-触发 false-没有触发
    void set_check_connect_alarm(void) {alarm(ALARM_TIME);};                            // 设置检查链接超时的定时器

private:
    ~ConnectManager() = default;
    ConnectManager() = default;

private:
    std::map<std::string, struct user_connect> d_user_connect;          // 保存用户-链接对
    std::map<uint, struct user_connect> d_unknown_connect;              // 保存一个未知的用户链接
    uint d_unknown_connect_num = 0;                                     // 未知的用户编号从 0 开始分配
    bool d_signal_trigger = false;                                      // 判断信号是否触发： true-触发 false-没有触发
};

#endif