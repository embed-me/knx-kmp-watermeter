#ifndef _ARDUINO_LOGGER_HPP_
#define _ARDUINO_LOGGER_HPP_

#include "../ILogger.hpp"

#include <ArduinoLog.h>
#include <string>

namespace drivers::logger {

class ArduinoLogger : public ILogger
{
public:
    ArduinoLogger();
    ~ArduinoLogger() = default;

    virtual void init(enum LOGLEVEL level, bool blockTillSerialConnected = false) override;
    virtual void setLoglevel(enum LOGLEVEL level) override;

    virtual void fatal(const char* msg, ...) override;
    virtual void fatal(const char* msg, va_list args) override;
    virtual void fatal(std::string msg) override;

    virtual void error(const char* msg, ...) override;
    virtual void error(const char* msg, va_list args) override;
    virtual void error(std::string msg) override;

    virtual void warning(const char* msg, ...) override;
    virtual void warning(const char* msg, va_list args) override;
    virtual void warning(std::string msg) override;

    virtual void info(const char* msg, ...) override;
    virtual void info(const char* msg, va_list args) override;
    virtual void info(std::string msg) override;

    virtual void trace(const char* msg, ...) override;
    virtual void trace(const char* msg, va_list args) override;
    virtual void trace(std::string msg) override;

private:
    static void printPrefix(Print* _logOutput, int logLevel);
    uint8_t convertLoglevel(enum LOGLEVEL level);

    enum LOGLEVEL loglevel = LOGLEVEL_OFF;

    char msgBuffer[256];
};

}

#endif //_ARDUINO_LOGGER_HPP_