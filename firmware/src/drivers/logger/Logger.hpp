#ifndef _LOGGER_HPP_
#define _LOGGER_HPP_

#include <memory>
#include <optional>

#include "ILogger.hpp"
#include "DummyLogger.hpp"

namespace drivers::logger {

class Logger {
public:
    static void setLogger(std::shared_ptr<ILogger> customLogger)
    {
        logger = customLogger;
    }

    static std::shared_ptr<ILogger> getLogger() {
        if (!logger) {
            if (!dummyLogger) {
                dummyLogger = std::make_shared<DummyLogger>();
            }
            return dummyLogger;
        }
        return logger;
    };

private:
    ~Logger() = default;
    Logger() = default;

    static std::shared_ptr<ILogger> logger;
    static std::shared_ptr<ILogger> dummyLogger;
};


inline void logTrace(const std::string& message) {
    Logger::getLogger()->trace(message.c_str());
}

inline void logTrace(const char* msg, ...) {
    va_list args;
    va_start(args, msg);
    Logger::getLogger()->trace(msg, args);
    va_end(args); 
}

inline void logInfo(const std::string& message) {
    Logger::getLogger()->info(message.c_str());
}

inline void logInfo(const char* msg, ...) {
    va_list args;
    va_start(args, msg);
    Logger::getLogger()->info(msg, args);
    va_end(args); 
}

inline void logWarning(const std::string& message) {
    Logger::getLogger()->warning(message.c_str());
}

inline void logWarning(const char* msg, ...) {
    va_list args;
    va_start(args, msg);
    Logger::getLogger()->warning(msg, args);
    va_end(args); 
}

inline void logError(const std::string& message) {
    Logger::getLogger()->error(message.c_str());
}

inline void logError(const char* msg, ...) {
     va_list args;
    va_start(args, msg);
    Logger::getLogger()->error(msg, args);
    va_end(args); 
}

inline void logFatal(const std::string& message) {
    Logger::getLogger()->fatal(message.c_str());
}

inline void logFatal(const char* msg, ...) {
    va_list args;
    va_start(args, msg);
    Logger::getLogger()->fatal(msg, args);
    va_end(args); 
}


}

#endif //_LOGGER_HPP_