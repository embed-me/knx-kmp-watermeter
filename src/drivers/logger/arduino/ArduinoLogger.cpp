#include "ArduinoLogger.hpp"

#include <Arduino.h>

using namespace drivers::logger;

ArduinoLogger::ArduinoLogger()
{
    memset(msgBuffer, 0, sizeof(msgBuffer));
}

void ArduinoLogger::init(enum LOGLEVEL level, bool blockTillSerialConnected)
{
    Serial.begin(115200);
    
    if (blockTillSerialConnected) {
        while (!Serial);
    }


    auto arduinoLogLevel = convertLoglevel(level);
    Log.begin(arduinoLogLevel, &Serial);
}

void ArduinoLogger::setLoglevel(enum LOGLEVEL level)
{
    auto arduinoLogLevel = convertLoglevel(level);
    Log.setLevel(arduinoLogLevel);
}

void ArduinoLogger::fatal(const char* msg, ...)
{
    va_list args;
    va_start(args, msg);
    vsnprintf(msgBuffer, sizeof(msgBuffer), msg, args);
    va_end(args);

    Log.fatalln(msgBuffer);
}

void ArduinoLogger::fatal(std::string msg)
{
    this->fatal(msg.c_str());
}

void ArduinoLogger::fatal(const char* msg, va_list args)
{
    vsnprintf(msgBuffer, sizeof(msgBuffer), msg, args);
    Log.fatalln(msgBuffer);
}

void ArduinoLogger::error(const char* msg, ...)
{
    va_list args;
    va_start(args, msg);
    vsnprintf(msgBuffer, sizeof(msgBuffer), msg, args);
    va_end(args);

    Log.errorln(msgBuffer);
}

void ArduinoLogger::error(std::string msg)
{
    this->error(msg.c_str());
}

void ArduinoLogger::error(const char* msg, va_list args)
{
    vsnprintf(msgBuffer, sizeof(msgBuffer), msg, args);
    Log.errorln(msgBuffer);
}

void ArduinoLogger::warning(const char* msg, ...)
{
    va_list args;
    va_start(args, msg);
    vsnprintf(msgBuffer, sizeof(msgBuffer), msg, args);
    va_end(args);

    Log.warningln(msgBuffer);
}

void ArduinoLogger::warning(std::string msg)
{
    this->warning(msg.c_str());
}

void ArduinoLogger::warning(const char* msg, va_list args)
{
    vsnprintf(msgBuffer, sizeof(msgBuffer), msg, args);
    Log.warningln(msgBuffer);
}

void ArduinoLogger::info(const char* msg, ...)
{
    va_list args;
    va_start(args, msg);
    vsnprintf(msgBuffer, sizeof(msgBuffer), msg, args);
    va_end(args);

    Log.infoln(msgBuffer);
}

void ArduinoLogger::info(std::string msg)
{
    this->info(msg.c_str());
}

void ArduinoLogger::info(const char* msg, va_list args)
{
    vsnprintf(msgBuffer, sizeof(msgBuffer), msg, args);
    Log.infoln(msgBuffer);
}

void ArduinoLogger::trace(const char* msg, ...)
{
    va_list args;
    va_start(args, msg);
    vsnprintf(msgBuffer, sizeof(msgBuffer), msg, args);
    va_end(args);

    Log.traceln(msgBuffer);
}

void ArduinoLogger::trace(std::string msg)
{
    this->trace(msg.c_str());
}

void ArduinoLogger::trace(const char* msg, va_list args)
{
    vsnprintf(msgBuffer, sizeof(msgBuffer), msg, args);
    Log.traceln(msgBuffer);
}

uint8_t ArduinoLogger::convertLoglevel(enum LOGLEVEL level)
{
    uint8_t adruinoLoglevel = LOG_LEVEL_SILENT;

    switch(level) {
        case LOGLEVEL_OFF:
            adruinoLoglevel = LOG_LEVEL_SILENT;
            break;
        case LOGLEVEL_FATAL:
            adruinoLoglevel = LOG_LEVEL_FATAL;
            break;
        case LOGLEVEL_ERROR:
            adruinoLoglevel = LOG_LEVEL_ERROR;
            break;
        case LOGLEVEL_WARNING:
            adruinoLoglevel = LOG_LEVEL_WARNING;
            break;
        case LOGLEVEL_INFO:
            adruinoLoglevel = LOG_LEVEL_INFO;
            break;
        case LOGLEVEL_TRACE:
            adruinoLoglevel = LOG_LEVEL_TRACE;
            break;
        default:
            break;
    }

    return adruinoLoglevel;
}