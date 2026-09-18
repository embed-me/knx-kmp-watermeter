#include "ArduinoMotionDriver.hpp"

namespace drivers::motion {


ArduinoMotionDriver::ArduinoMotionDriver() {}


ArduinoMotionDriver::~ArduinoMotionDriver()
{
    if (servo_.attached()) {
        servo_.detach();
    }
}

void ArduinoMotionDriver::setConfig(const MotionConfig& cfg)
{
    pin_ = cfg.pin;
    servo_.attach(pin_, cfg.minPulseWidth, cfg.maxPulseWidth);
    servo_.write(cfg.sleepAngle);
    configured_ = true;
}

void ArduinoMotionDriver::write(uint8_t angle)
{
    if (!configured_) {
        return;
    }
    
    servo_.write(angle);
}

}
