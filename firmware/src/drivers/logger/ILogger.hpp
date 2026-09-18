#ifndef _I_LOGGER_HPP_
#define _I_LOGGER_HPP_

#include <cstdarg>
#include <string>

namespace drivers::logger {

enum LOGLEVEL {
    LOGLEVEL_OFF = 0,
    LOGLEVEL_FATAL = 1,
    LOGLEVEL_ERROR = 2,
    LOGLEVEL_WARNING = 3,
    LOGLEVEL_INFO = 4,
    LOGLEVEL_TRACE = 5
};

class ILogger {
public:
    virtual ~ILogger() = default;

    virtual void init(enum LOGLEVEL level, bool blockTillSerialConnected = false) = 0;

    virtual void setLoglevel(enum LOGLEVEL level) = 0;

    virtual void fatal(const char* msg, ...) = 0;
    virtual void fatal(const char* msg, va_list args) = 0;
    virtual void fatal(std::string msg) = 0;

    virtual void error(const char* msg, ...) = 0;
    virtual void error(const char* msg, va_list args) = 0;
    virtual void error(std::string msg) = 0;

    virtual void warning(const char* msg, ...) = 0;
    virtual void warning(const char* msg, va_list args) = 0;
    virtual void warning(std::string msg) = 0;

    virtual void info(const char* msg, ...) = 0;
    virtual void info(const char* msg, va_list args) = 0;
    virtual void info(std::string msg) = 0;

    virtual void trace(const char* msg, ...) = 0;
    virtual void trace(const char* msg, va_list args) = 0;
    virtual void trace(std::string msg) = 0;
};

}

#endif //_I_LOGGER_HPP_