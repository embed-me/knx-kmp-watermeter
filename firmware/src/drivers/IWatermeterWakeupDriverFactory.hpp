#ifndef _I_WATERMETER_WAKEUP_DRIVER_FACTORY_HPP_
#define _I_WATERMETER_WAKEUP_DRIVER_FACTORY_HPP_

#include "watermeter/wakeup/IWatermeterWakeupDriver.hpp"
#include "motion/IMotionDriver.hpp"
#include "motion/MotionConfig.hpp"
#include <memory>

namespace drivers {

class IWatermeterWakeupDriverFactory {
public:
    ~IWatermeterWakeupDriverFactory() = default;

    virtual std::shared_ptr<watermeter::wakeup::IWatermeterWakeupDriver> getWatermeterWakeupDriver(
        std::shared_ptr<motion::IMotionDriver> motion,
        const motion::MotionConfig& cfg) = 0;
};

}

#endif //_I_WATERMETER_WAKEUP_DRIVER_FACTORY_HPP_
