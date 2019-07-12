#include <fstream>
#include <memory>
#include <cstring>
#include <cerrno>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#include "server_log.h"

Log&
Log::get_instance(void)
{
    static Log log_instance(LOG_PATH);
    return log_instance;
}

Log::Log(const std::string& log_dir_path)
{
    if (opendir(log_dir_path.c_str()) == NULL) {
        if (errno == ENOENT) {
            int ret = mkdir(log_dir_path.c_str(), S_IRUSR | S_IWUSR | S_IXUSR);
            if (ret == -1) {
                fprintf(stderr, "Log:: Log mkdir calls failed: %s\n", strerror(errno));
            }
        } else {
            fprintf(stderr, "Log:: opendir calls failed: %s\n", strerror(errno));
        }
    }
    log4cxx::PropertyConfigurator::configure(LOG_PROPERTY_PATH);
    d_server_logger = log4cxx::Logger::getLogger("server_file");
    d_console_logger = log4cxx::Logger::getLogger("server_console");
}

Log::~Log()
{

}

void
Log::log_msg(int log_severity,int line_num, const char *file_name, const char *format, ...)
{
    char log_msg_buffer[LOG_MSG_BUFFER];
    memset(log_msg_buffer,'\0',sizeof(log_msg_buffer));

    sprintf(log_msg_buffer, "[%s(%d)]: ", basename(file_name), line_num);
    std::string log_msg = std::string(log_msg_buffer);
    memset(log_msg_buffer,'\0',sizeof(log_msg_buffer));

    va_list arg_ptr;
    va_start(arg_ptr,format);

    vsprintf(log_msg_buffer, format, arg_ptr);
    log_msg = log_msg + log_msg_buffer;

    switch(log_severity){
        case LOG_INFO :
            LOG4CXX_INFO(d_server_logger, log_msg);
            break;
        case LOG_WARN :
            LOG4CXX_WARN(d_server_logger, log_msg);
            break;
        case LOG_ERROR :
            LOG4CXX_ERROR(d_server_logger, log_msg);
            break;
        case LOG_FATAL :
            LOG4CXX_FATAL(d_server_logger, log_msg);
            break;
        case LOG_CONSOLE:
            std::cout << log_msg << std::endl;
            break;
        default:
            break;
    }
    va_end(arg_ptr);
}