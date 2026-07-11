#ifndef _I_MOTION_DRIVER_FACTORY_HPP_
#define _I_MOTION_DRIVER_FACTORY_HPP_

#include "motion/IMotionDriver.hpp"
#include <memory>

namespace drivers {

class IMotionDriverFactory {
public:
    ~IMotionDriverFactory() = default;

    virtual std::shared_ptr<motion::IMotionDriver> getMotionDriver() = 0;
};

}

#endif //_I_MOTION_DRIVER_FACTORY_HPP_
