#ifndef __MSG_TYPE__
#define __MSG_TYPE__

enum server_respone_client {
    SIGN_UP_SUCCESSED = 128,
    SIGN_OUT_SUCCESSED,
    SIGN_IN_SUCCESSED,
    CREATE_NEW_GROUP_SUCCESSED,
    UPDATE_USER_NAME_SUCCESSED,
    UPDATE_USER_PASSWORD_SUCCESSED,
    UPDATE_GROUP_NAME_SUCCESSED,
    UPDATE_GROUP_MANAGER_SUCCESSED,
    REMOVE_A_GROUP_SUCCESSED,
    SERVER_SEND_MSG_TO_USER,
    SERVER_SEND_USER_INFO,
    SERVER_SEND_GROUP_INFO,
    SERVER_SEND_USER_FRIENDS_INFO,
    SERVER_SEND_USER_MANAGER_GROUPS_INFO,
    SERVER_SEND_USER_JOINED_GROUPS_INFO,
    SERVER_NOTIFY_USER_THE_RESULT_OF_JOIN_GROUP_REQUEST_SUCCESSED,
    SERVER_NOTIFY_USER_THE_RESULT_OF_JOIN_GROUP_REQUEST_FAILED,
    SERVER_NOTIFY_USER_THE_RESULT_OF_BE_FRIEND_REQUEST_SUCCESSED,
    SERVER_NOTIFY_USER_THE_RESULT_OF_BE_FRIEND_REQUEST_FAILED,
    SERVER_NOTIFY_USER_THE_RESULT_OF_REMOVE_FRIEND,
    SERVER_NOTIFY_GORUP_THE_RESULT_OF_MOVED_MEMBER,
};

enum client_request_server {
    SIGN_UP = 0,
    SIGN_OUT,
    SIGN_IN,
    CREATE_NEW_GROUP,
    UPDATE_USER_NAME,
    UPDATE_USER_PASSWORD,
    UPDATE_GROUP_NAME,
    UPDATE_GROUP_MANAGER,
    REQUEST_JOIN_THE_GROUP,
    REQUEST_BE_FRIEND,
    REQUEST_USER_INFO,
    REQUEST_GROUP_INFO,
    GROUP_MANAGER_RESPONE_JOIN_GROUP_REQUEST_AGREE,
    GROUP_MANAGER_RESPONE_JOIN_GROUP_REQUEST_DISAGREE,
    USER_RESPONE_FRIEND_REQUEST_AGREE,
    USER_RESPONE_FRIEND_REQUEST_DISAGREE,
    USER_QUIT_FROM_GROUP,
    REMOVE_A_FRIEND,
    GROUP_MANAGER_REMOVE_A_MEMBER,
    REMOVE_A_GROUP,  //使用此操作server像所有用户发送SERVER_NOTIFY_USER_WHO_HAS_BEEN_MOVED_FROM_GROUP
    SEND_MSG_TO_FRIEND,
    SEND_MSG_TO_GROUP_MEMBERS,
    SEND_MSG_TO_SINGLE_GROUP_MEMBER,
};

#endif