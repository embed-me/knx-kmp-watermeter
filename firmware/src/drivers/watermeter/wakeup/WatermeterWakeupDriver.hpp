#ifndef _WATERMETER_WAKEUP_DRIVER_HPP_
#define _WATERMETER_WAKEUP_DRIVER_HPP_

#include "IWatermeterWakeupDriver.hpp"
#include "src/drivers/motion/IMotionDriver.hpp"
#include "src/drivers/motion/MotionConfig.hpp"
#include "src/drivers/timer/ITimer.hpp"

#include <memory>
#include <functional>

namespace drivers::watermeter::wakeup {

class WatermeterWakeupDriver : public IWatermeterWakeupDriver {
public:
    WatermeterWakeupDriver(std::shared_ptr<motion::IMotionDriver> motion,
                           const motion::MotionConfig& cfg,
                           std::shared_ptr<drivers::timer::ITimer> settleTimer);
    ~WatermeterWakeupDriver() override = default;

    void wakeup(std::function<void()> onReady) override;
    void sleep() override;

private:
    void writeMotion(uint8_t angle);
    void onSettleDone();

    std::shared_ptr<motion::IMotionDriver> motion_;
    uint8_t wakeupAngle_;
    uint8_t sleepAngle_;
    uint32_t settleDelayUs_;

    std::shared_ptr<drivers::timer::ITimer> settleTimer_;
    std::function<void()> onReady_;
    bool settling_ = false;
    bool awake_ = false;
};

}

#endif //_WATERMETER_WAKEUP_DRIVER_HPP_
