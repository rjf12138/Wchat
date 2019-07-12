#include "msg_handler.h"
#include "msg_fragment.h"
#include "server_log.h"
#include "queue.h"
#include "msg_buffer.h"
#include "msg_type.h"
#include "fragment_operate.h"

MsgHandler&
MsgHandler::get_msg_handler_instance(UserManager& user_manager_instance, GroupManager& group_manager_instance)
{
    static MsgHandler msg_handler_instance(user_manager_instance, group_manager_instance);
    return msg_handler_instance;
}

MsgHandler::MsgHandler(UserManager& user_manager_instance, GroupManager& group_manager_instance)
                            :user_manager(user_manager_instance), group_manager(group_manager_instance)
{

}

int
MsgHandler::handle_message(void)
{
    struct fragment tmp_frag;

    if (FRAGMENT_RECV_SIZE() > 0) {
        FRAGMENT_RECV_POP(tmp_frag);
        switch (tmp_frag.msg_type) {
            case SEND_MSG_TO_FRIEND:
            {
                if (user_manager.is_user_online(std::string(tmp_frag.receiver_account))) {
                    FRAGMENT_SEND_PUSH(tmp_frag);
                } else {
                    MSG_BUFFER_PUSH(tmp_frag);
                }
            }break;
            case SEND_MSG_TO_GROUP_MEMBERS:
            {
                struct group group_member_info;
                if (group_manager.get_group_info(std::string(tmp_frag.group_account), group_member_info) == -1) {
                    LOG_WARN("MsgHandler::handle_message() failed: GroupManager::get_group_info() calls failed.");
                    return -1;
                }

                for (const auto& member_account : group_member_info.member_accounts) {
                    FragmentOperate::set_receiver_account(tmp_frag, member_account);
                    FragmentOperate::transmit_fragment(tmp_frag, user_manager.is_user_online(member_account));
                }
            }break;
            case REQUEST_JOIN_THE_GROUP:
            {
                struct group t_group_info;

                if (group_manager.get_group_info(std::string(tmp_frag.group_account), t_group_info) == -1) {
                    LOG_ERROR("MsgHandler::handle_message() failed: GroupManager::get_group_info() calls failed.");
                    return -1;
                }
                FragmentOperate::set_receiver_account(tmp_frag, t_group_info.group_manager_account);
                FragmentOperate::transmit_fragment(tmp_frag, user_manager.is_user_online(t_group_info.group_manager_account));
            } break;
            case REQUEST_BE_FRIEND:
            {
                FragmentOperate::transmit_fragment(tmp_frag, user_manager.is_user_online(std::string(tmp_frag.receiver_account)));
            } break;
            default: 
            {
                FRAGMENT_INNER_TRANSMISSON_PUSH(tmp_frag);
            } break;
        }
    }
}
