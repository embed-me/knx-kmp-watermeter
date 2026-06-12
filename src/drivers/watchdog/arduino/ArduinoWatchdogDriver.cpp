#include "ArduinoWatchdogDriver.hpp"

#include <hardware/watchdog.h>

using namespace drivers::watchdog;

ArduinoWatchdogDriver::ArduinoWatchdogDriver()
{
}

void ArduinoWatchdogDriver::feed()
{
    watchdog_update();
}

void ArduinoWatchdogDriver::enable(uint32_t timeout_ms)
{
    bool isPausedOnDebug = true;
    watchdog_enable(timeout_ms, isPausedOnDebug);
}