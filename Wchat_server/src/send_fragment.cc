#include <stdlib.h>

#include "send_fragment.h"

SendMsg::SendMSG()
{
    if (server_send_fd = socket(AF_INET, SOCK_STREAM, 0) == -1) {
        LOG_ERROR("SendMsg:: socket error: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }
}