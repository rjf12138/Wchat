#ifndef __LOG_H__
#define __LOG_H__

#include <iostream>
#include <string>

#include <cstdarg>
#include <cstring>
#include <cstdio>
#include <memory>
#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/helpers/exception.h>
#include <log4cxx/propertyconfigurator.h>

#define LOG_PROPERTY_PATH       "../../server_config/log_property"
#define LOG_PATH                "./log"
#define LOG_MSG_BUFFER          256

enum log_serverity {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_FATAL,
    LOG_CONSOLE
};


class Log{
public:
    static Log& get_instance(void);
    
    Log(const std::string& p_log_dir_path);
    void log_msg(int log_severity,int line_num, const char *file_name, const char *format, ...);

    ~Log();
private:
    Log() = default;
private:
    log4cxx::LoggerPtr d_server_logger;
    log4cxx::LoggerPtr d_console_logger;
};

#define LOG_INFO(...)       Log::get_instance().log_msg(LOG_INFO, __LINE__, __FILE__,  __VA_ARGS__)
#define LOG_WARN(...)       Log::get_instance().log_msg(LOG_WARN, __LINE__, __FILE__, __VA_ARGS__)
#define LOG_ERROR(...)      Log::get_instance().log_msg(LOG_ERROR, __LINE__, __FILE__, __VA_ARGS__)
#define LOG_FATAL(...)      Log::get_instance().log_msg(LOG_FATAL, __LINE__, __FILE__, __VA_ARGS__)

#define LOG_CONSOLE(...)     Log::get_instance().log_msg(LOG_CONSOLE, __LINE__, __FILE__, __VA_ARGS__)

#endif