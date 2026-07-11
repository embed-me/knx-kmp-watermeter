#ifndef _WATERMETER_WAKEUP_DRIVER_HPP_
#define _WATERMETER_WAKEUP_DRIVER_HPP_

#include "IWatermeterWakeupDriver.hpp"
#include "src/drivers/motion/IMotionDriver.hpp"
#include "src/drivers/motion/MotionConfig.hpp"

#include <memory>

namespace drivers::watermeter::wakeup {

class WatermeterWakeupDriver : public IWatermeterWakeupDriver {
public:
    WatermeterWakeupDriver(std::shared_ptr<motion::IMotionDriver> motion,
                           const motion::MotionConfig& cfg);
    ~WatermeterWakeupDriver() override = default;

    void wakeup() override;
    void sleep() override;
    bool isAwake() const override;

private:
    std::shared_ptr<motion::IMotionDriver> motion_;
    bool isAwake_ = false;
    uint8_t wakeupAngle_;
    uint8_t sleepAngle_;
};

}

#endif //_WATERMETER_WAKEUP_DRIVER_HPP_
