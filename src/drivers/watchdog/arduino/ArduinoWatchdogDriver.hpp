#ifndef _ARDUINO_WATCHDOG_DRIVER_HPP_
#define _ARDUINO_WATCHDOG_DRIVER_HPP_

#include "../IWatchdogDriver.hpp"

#include <memory>

namespace drivers::watchdog {

class ArduinoWatchdogDriver : public IWatchdogDriver
{
public:
    ArduinoWatchdogDriver();
    ~ArduinoWatchdogDriver() = default;

    virtual void feed() override;
    virtual void enable(uint32_t timeout_ms) override;
};

}

#endif //_ARDUINO_WATCHDOG_DRIVER_HPP_ 