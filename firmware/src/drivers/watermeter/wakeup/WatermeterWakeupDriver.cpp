#include "WatermeterWakeupDriver.hpp"
#include "src/drivers/logger/Logger.hpp"

using namespace drivers::motion;
using namespace drivers::logger;

namespace drivers::watermeter::wakeup {

WatermeterWakeupDriver::WatermeterWakeupDriver(std::shared_ptr<IMotionDriver> motion,
                                               const MotionConfig& cfg)
    : motion_(motion)
    , wakeupAngle_(cfg.wakeupAngle)
    , sleepAngle_(cfg.sleepAngle)
{
}

void WatermeterWakeupDriver::wakeup()
{
    if (motion_) {
        logInfo("Waking up watermeter");
        motion_->write(wakeupAngle_);
        isAwake_ = true;
    }
}

void WatermeterWakeupDriver::sleep()
{
    if (motion_) {
        logInfo("Putting watermeter to sleep");
        motion_->write(sleepAngle_);
        isAwake_ = false;
    }
}

bool WatermeterWakeupDriver::isAwake() const
{
    return isAwake_;
}

}
