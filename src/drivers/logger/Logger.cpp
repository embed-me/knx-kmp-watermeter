#include "Logger.hpp"

using namespace drivers::logger;

std::shared_ptr<ILogger> Logger::dummyLogger = nullptr;
std::shared_ptr<ILogger> Logger::logger = nullptr;