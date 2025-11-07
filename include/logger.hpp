#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <fstream>
#include <iostream>

namespace image_processor
{

    enum class LogLevel
    {
        IP_LOGLV_DEBUG,
        IP_LOGLV_INFO,
        IP_LOGLV_WARNING,
        IP_LOGLV_ERROR
    };

    class Logger
    {
    public:
        static void log(LogLevel level, const std::string &message);
        static void setLogFile(const std::string &filepath);
        static void setConsoleOutput(bool enable);

    private:
        static std::ofstream log_file_;
        static bool console_output_;
    };

} // namespace image_processor

#endif