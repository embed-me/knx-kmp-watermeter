#include "ArduinoTimerFactory.hpp"
#include "arduino/ArduinoTimer.hpp"
#include "src/drivers/logger/Logger.hpp"

using namespace drivers::timer;
using namespace drivers::logger;

ArduinoTimerFactory::ArduinoTimerFactory()
{
}

std::shared_ptr<ITimer> ArduinoTimerFactory::getTimer()
{
    logTrace("creating new Timer");
    auto timer = std::make_shared<ArduinoTimer>();
    return timer;
}