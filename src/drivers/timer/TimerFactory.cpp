#include "TimerFactory.hpp"
#include "arduino/ArduinoTimer.hpp"
#include "src/drivers/logger/Logger.hpp"

using namespace drivers::timer;
using namespace drivers::logger;

TimerFactory::TimerFactory()
{
}

std::shared_ptr<ITimer> TimerFactory::getTimer()
{
    logTrace("creating new Timer");
    auto timer = std::make_shared<ArduinoTimer>();
    return timer;
}