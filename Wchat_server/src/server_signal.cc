#include <map>
#include <functional>
#include <signal.h>

#include "server_log.h"
#include "server_signal.h"
#include "msg_transmission.h"

std::map<int, std::function<void()>> handlers;
void signal_handler(int sig) {
    handlers[sig]();
}

void
Signal::signal(int sig, const std::function<void()> &handler) {
    handlers[sig] = handler;
    ::signal(sig, signal_handler);
}

static bool exit_status = false;

void
Exit::init_exit(void)
{
    Signal::signal(SIGINT, []{MsgTransmission::exit_transmission(); exit_status = true;});
}

bool
Exit::stop(void)
{
    return exit_status;
}