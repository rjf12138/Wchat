#include "msg_util.h"

struct fragment
MsgUtil::generate_fragment(uint8_t p_msg_type, std::string data1, std::string data2)        // 确保 data 不要超过 frag.data 的长度
{
    struct fragment l_frag;
    memset(&l_frag, 0, sizeof(l_frag));

    switch(p_msg_type) {
        case SIGN_UP:
        {
            FragmentOperate::set_msg_type(l_frag, SIGN_UP);
            FragmentOperate::set_data(l_frag, data1);
        } break;
        case SIGN_IN:
        {
            FragmentOperate::set_msg_type(l_frag, SIGN_IN);
            FragmentOperate::set_sender_account(l_frag, data1);
            FragmentOperate::set_data(l_frag, data2);
        } break;
        case SIGN_OUT:
        {
            FragmentOperate::set_msg_type(l_frag, SIGN_OUT);
            FragmentOperate::set_sender_account(l_frag, data1);
        } break;
        case CREATE_NEW_GROUP:
        {
            FragmentOperate::set_msg_type(l_frag, CREATE_NEW_GROUP);
            FragmentOperate::set_sender_account(l_frag, data1);
            FragmentOperate::set_data(l_frag, data2);
        } break;
        case GROUP_MANAGER_RESPONE_JOIN_GROUP_REQUEST_AGREE:
        {
            FragmentOperate::set_msg_type(l_frag, GROUP_MANAGER_RESPONE_JOIN_GROUP_REQUEST_AGREE);
            FragmentOperate::set_sender_account(l_frag, data1);
            FragmentOperate::set_group_account(l_frag, data2);
        } break;
        case GROUP_MANAGER_RESPONE_JOIN_GROUP_REQUEST_DISAGREE:
        {
            FragmentOperate::set_msg_type(l_frag, GROUP_MANAGER_RESPONE_JOIN_GROUP_REQUEST_DISAGREE);
            FragmentOperate::set_sender_account(l_frag, data1);
            FragmentOperate::set_group_account(l_frag, data2);
        } break;
        case USER_RESPONE_FRIEND_REQUEST_AGREE:
        {
            FragmentOperate::set_msg_type(l_frag, USER_RESPONE_FRIEND_REQUEST_AGREE);
            FragmentOperate::set_sender_account(l_frag, data1);
            FragmentOperate::set_receiver_account(l_frag, data2);
        } break;
        case USER_RESPONE_FRIEND_REQUEST_DISAGREE:
        {
            FragmentOperate::set_msg_type(l_frag, USER_RESPONE_FRIEND_REQUEST_DISAGREE);
            FragmentOperate::set_sender_account(l_frag, data1);
            FragmentOperate::set_receiver_account(l_frag, data2);
        } break;
        case USER_QUIT_FROM_GROUP:
        {
            FragmentOperate::set_msg_type(l_frag, USER_QUIT_FROM_GROUP);
            FragmentOperate::set_group_account(l_frag, data1);
        } break;
        case REMOVE_A_FRIEND:
        {
            FragmentOperate::set_msg_type(l_frag, REMOVE_A_FRIEND);
            FragmentOperate::set_sender_account(l_frag, data1);
            FragmentOperate::set_receiver_account(l_frag, data2);
        } break;
        case GROUP_MANAGER_REMOVE_A_MEMBER:
        {
            FragmentOperate::set_msg_type(l_frag, GROUP_MANAGER_REMOVE_A_MEMBER);
            FragmentOperate::set_sender_account(l_frag, data1);
            FragmentOperate::set_data(l_frag, data2);
        } break;
        case REMOVE_A_GROUP:
        {
            FragmentOperate::set_msg_type(l_frag, REMOVE_A_GROUP);
            FragmentOperate::set_sender_account(l_frag, data1);
            FragmentOperate::set_group_account(l_frag, data2);
        } break;
        case REQUEST_USER_INFO:
        {
            FragmentOperate::set_msg_type(l_frag, REQUEST_USER_INFO);
            FragmentOperate::set_sender_account(l_frag, data1);
            FragmentOperate::set_data(l_frag, data2);
        } break;
        case REQUEST_GROUP_INFO:
        {
            FragmentOperate::set_msg_type(l_frag, REQUEST_GROUP_INFO);
            FragmentOperate::set_sender_account(l_frag, data1);
            FragmentOperate::set_group_account(l_frag, data2);
        } break;
    };

    return l_frag;
}

std::string
MsgUtil::print_msg_type(uint8_t p_msg_type)
{
    std::string l_msg_type;
    switch(p_msg_type) {
        case SIGN_UP_SUCCESSED:
        {
            l_msg_type = "SIGN_UP_SUCCESSED";
        } break;
        case SIGN_IN_SUCCESSED:
        {
            l_msg_type = "SIGN_IN_SUCCESSED";
        } break;
        case SIGN_OUT_SUCCESSED:
        {
            l_msg_type = "SIGN_OUT_SUCCESSED";
        } break;
        case CREATE_NEW_GROUP_SUCCESSED:
        {
            l_msg_type = "CREATE_NEW_GROUP_SUCCESSED";
        } break;
        case SERVER_NOTIFY_USER_THE_RESULT_OF_JOIN_GROUP_REQUEST_SUCCESSED:
        {
            l_msg_type = "SERVER_NOTIFY_USER_THE_RESULT_OF_JOIN_GROUP_REQUEST_SUCCESSED";
        } break;
        case SERVER_NOTIFY_USER_THE_RESULT_OF_JOIN_GROUP_REQUEST_FAILED:
        {
            l_msg_type = "SERVER_NOTIFY_USER_THE_RESULT_OF_JOIN_GROUP_REQUEST_FAILED";
        } break;
        case SERVER_NOTIFY_USER_THE_RESULT_OF_BE_FRIEND_REQUEST_SUCCESSED:
        {
            l_msg_type = "SERVER_NOTIFY_USER_THE_RESULT_OF_BE_FRIEND_REQUEST_SUCCESSED";
        } break;
        case SERVER_NOTIFY_USER_THE_RESULT_OF_BE_FRIEND_REQUEST_FAILED:
        {
            l_msg_type = "SERVER_NOTIFY_USER_THE_RESULT_OF_BE_FRIEND_REQUEST_FAILED";
        } break;
        case SERVER_NOTIFY_GORUP_THE_RESULT_OF_MOVED_MEMBER:
        {
            l_msg_type = "SERVER_NOTIFY_GORUP_THE_RESULT_OF_MOVED_MEMBER";
        } break;
        case SERVER_NOTIFY_USER_THE_RESULT_OF_REMOVE_FRIEND:
        {
            l_msg_type = "SERVER_NOTIFY_USER_THE_RESULT_OF_REMOVE_FRIEND";
        } break;
        case GROUP_MANAGER_REMOVE_A_MEMBER:
        {
            l_msg_type = "SERVER_NOTIFY_GORUP_THE_RESULT_OF_MOVED_MEMBER";
        } break;
        case REMOVE_A_GROUP:
        {
            l_msg_type = "REMOVE_A_GROUP_SUCCESSED";
        } break;
        case REQUEST_USER_INFO:
        {
            l_msg_type = "SERVER_SEND_USER_INFO";
        } break;
        case REQUEST_GROUP_INFO:
        {
            l_msg_type = "SERVER_SEND_GROUP_INFO";
        } break;
        default:
        {
            std::cout << "unknown msg type: " << p_msg_type << std::endl; 
        } break;
    };
    return l_msg_type;
}

void 
MsgUtil::print_fragment(const struct fragment& p_frag)
{
    std::cout << "msg_type: " << print_msg_type(p_frag.msg_type) << std::endl;
    std::cout << "send_account: " << p_frag.sender_account << std::endl;
    std::cout << "receiver_account: " << p_frag.receiver_account << std::endl;
    std::cout << "group_account: " << p_frag.group_account << std::endl;
    std::cout << "fragment_amount: " << p_frag.fragment_amount << std::endl;
    std::cout << "fragment_position: " << p_frag.fragment_position << std::endl;
    std::cout << "fragment_length: " << p_frag.fragment_length << std::endl;
    std::cout << "identity: " << p_frag.identity << std::endl;
    std::cout << "data: " << p_frag.data << std::endl;
}