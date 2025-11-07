#include "logger.hpp"
#include <ctime>
#include <iomanip>
#include <sstream>

namespace image_processor
{

    std::ofstream Logger::log_file_;
    bool Logger::console_output_ = true;

    static std::string getTimeString()
    {
        std::ostringstream oss;
        std::time_t t = std::time(nullptr);
        std::tm tm;
#ifdef _WIN32
        localtime_s(&tm, &t);
#else
        localtime_r(&t, &tm);
#endif
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

    static std::string logLevelToString(LogLevel level)
    {
        switch (level)
        {
        case LogLevel::IP_LOGLV_DEBUG:
            return "DEBUG";
        case LogLevel::IP_LOGLV_INFO:
            return "INFO";
        case LogLevel::IP_LOGLV_WARNING:
            return "WARNING";
        case LogLevel::IP_LOGLV_ERROR:
            return "ERROR";
        default:
            return "UNKNOWN";
        }
    }

    void Logger::log(LogLevel level, const std::string &message)
    {
        std::string log_line = "[" + getTimeString() + "] [" + logLevelToString(level) + "] " + message;
        if (console_output_)
        {
            std::cout << log_line << std::endl;
        }
        if (log_file_.is_open())
        {
            log_file_ << log_line << std::endl;
        }
    }

    void Logger::setLogFile(const std::string &filepath)
    {
        if (log_file_.is_open())
        {
            log_file_.close();
        }
        log_file_.open(filepath, std::ios::app);
    }

    void Logger::setConsoleOutput(bool enable = true)
    {
        console_output_ = enable;
    }

} // namespace image_processor