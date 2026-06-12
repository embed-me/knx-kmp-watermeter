#ifndef _ARDUINO_GPIO_DRIVER_HPP_
#define _ARDUINO_GPIO_DRIVER_HPP_

#include "../IGpioDriver.hpp"
#include "src/drivers/logger/Logger.hpp"

#include <memory>
#include "Arduino.h"

namespace drivers::gpio {

class ArduinoGpioDriver : public IGpioDriver
{
public:
    ArduinoGpioDriver();
    ~ArduinoGpioDriver() = default;

    virtual void setConfig(const GpioConfig &gpio) override;
    virtual bool readValue(const GpioConfig &gpio) override;
    virtual bool writeValue(const GpioConfig &gpio, bool value) override;
    virtual bool setupInterruptHandler(const GpioConfig &gpio, IsrFunctionPtr handler) override;
private:
    PinMode convertGpioMode(const GpioConfig &gpio);
};

}

#endif //_ARDUINO_GPIO_DRIVER_HPP_