#ifndef _MOTION_CONFIG_HPP_
#define _MOTION_CONFIG_HPP_

#include <cstdint>

namespace drivers::motion {

struct MotionConfig {
    uint8_t pin;
    uint16_t minPulseWidth;
    uint16_t maxPulseWidth;
    uint8_t wakeupAngle;
    uint8_t sleepAngle;
};

}

#endif //_MOTION_CONFIG_HPP_
