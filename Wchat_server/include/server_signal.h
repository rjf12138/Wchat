#ifndef __SERVER_SIGNAL_H__
#define __SERVER_SIGNAL_H__

#include <functional>
#include <signal.h>

struct Signal {
    static void signal(int sig, const std::function<void()> &handler);
};

struct Exit {
public:
    static void init_exit(void);
    static bool stop(void);
    static void exit_sig_handler(int sig);
};

#endif