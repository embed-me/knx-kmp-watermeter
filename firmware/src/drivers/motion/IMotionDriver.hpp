#ifndef _I_MOTION_DRIVER_HPP_
#define _I_MOTION_DRIVER_HPP_

#include "MotionConfig.hpp"
#include <cstdint>

namespace drivers::motion {

class IMotionDriver {
public:
    virtual ~IMotionDriver() = default;

    virtual void setConfig(const MotionConfig& cfg) = 0;
    virtual void write(uint8_t angle) = 0;
};

}

#endif //_I_MOTION_DRIVER_HPP_
