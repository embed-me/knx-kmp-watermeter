#include "WatermeterWakeupDriver.hpp"
#include "src/drivers/logger/Logger.hpp"
#include "src/utils/scheduler/Scheduler.hpp"

using namespace drivers::motion;
using namespace drivers::logger;

namespace drivers::watermeter::wakeup {

WatermeterWakeupDriver::WatermeterWakeupDriver(std::shared_ptr<IMotionDriver> motion,
                                               const MotionConfig& cfg,
                                               std::shared_ptr<drivers::timer::ITimer> settleTimer)
    : motion_(motion)
    , wakeupAngle_(cfg.wakeupAngle)
    , sleepAngle_(cfg.sleepAngle)
    , settleDelayUs_(cfg.settleDelayUs)
    , settleTimer_(settleTimer)
{
    settleTimer_->setupInterruptHandler([](void* arg) {
        auto* self = static_cast<WatermeterWakeupDriver*>(arg);
        utils::Scheduler::schedule([self](void*) {
            self->onSettleDone();
        });
    }, this);
}

void WatermeterWakeupDriver::writeMotion(uint8_t angle)
{
    if (motion_) {
        motion_->write(angle);
    }
}

void WatermeterWakeupDriver::wakeup(std::function<void()> onReady)
{
    if (settling_) {
        return;
    }
    if (awake_) {
        if (onReady) onReady();
        return;
    }

    onReady_ = onReady;
    logInfo("Waking up watermeter");
    writeMotion(wakeupAngle_);
    settling_ = true;
    settleTimer_->start(settleDelayUs_, drivers::timer::TimerMode::SINGLE_SHOT);
}

void WatermeterWakeupDriver::sleep()
{
    logInfo("Putting watermeter to sleep");
    writeMotion(sleepAngle_);
    awake_ = false;
}

void WatermeterWakeupDriver::onSettleDone()
{
    settling_ = false;
    awake_ = true;
    if (onReady_) {
        auto cb = onReady_;
        onReady_ = nullptr;
        cb();
    }
}

}
