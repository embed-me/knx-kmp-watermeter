#ifndef _ARDUINO_MOTION_DRIVER_HPP_
#define _ARDUINO_MOTION_DRIVER_HPP_

#include "../IMotionDriver.hpp"
#include <Servo.h>

namespace drivers::motion {

class ArduinoMotionDriver : public IMotionDriver {
public:
    ArduinoMotionDriver();
    ~ArduinoMotionDriver() override;

    void setConfig(const MotionConfig& cfg) override;
    void write(uint8_t angle) override;

private:
    Servo servo_;
    bool configured_ = false;
    uint8_t pin_ = 0;
};

}

#endif //_ARDUINO_MOTION_DRIVER_HPP_
