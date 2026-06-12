#include "ArduinoGpioDriver.hpp"
#include "src/drivers/logger/Logger.hpp"

using namespace drivers::gpio;
using namespace drivers::logger;

ArduinoGpioDriver::ArduinoGpioDriver()
{
}


PinMode ArduinoGpioDriver::convertGpioMode(const GpioConfig &gpio) 
{
    PinMode mode = PinMode::OUTPUT;

    if (gpio.direction == GPIO_DIRECTION::DIRECTION_INPUT) {
        mode = PinMode::INPUT;

        switch (gpio.pull)
        {
        case GPIO_PULL::PULL_HIGH:
            mode = PinMode::INPUT_PULLUP;
            break;
        case GPIO_PULL::PULL_LOW:
            mode = PinMode::INPUT_PULLDOWN;
            break;
        default:
            logWarning("Failed to convert GPIO Mode!");
            break;
        }
    } 

    return mode;
}

void ArduinoGpioDriver::setConfig(const GpioConfig &gpio)
{
    pinMode(gpio.pin, convertGpioMode(gpio));
}

bool ArduinoGpioDriver::readValue(const GpioConfig &gpio)
{
    return digitalRead(gpio.pin);
}

bool ArduinoGpioDriver::writeValue(const GpioConfig &gpio, bool value)
{
    if (gpio.direction != GPIO_DIRECTION::DIRECTION_OUTPUT) {
        logError("Trying to write to INPUT!");
        return false;
    }

    digitalWrite(gpio.pin, (int)value);
    return true;
}

bool ArduinoGpioDriver::setupInterruptHandler(const GpioConfig &gpio, IsrFunctionPtr handler)
{
    if (gpio.sensitivity != GPIO_SENSITIVITY::INTERRUPT_CHANGE) {
        logError("GPIO sensitivity does not support interrupts!");
        return false;
    }

    attachInterrupt(gpio.pin, handler, PinStatus::CHANGE);
    return true;
}