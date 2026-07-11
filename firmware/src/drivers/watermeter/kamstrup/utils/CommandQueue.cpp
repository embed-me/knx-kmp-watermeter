#include "CommandQueue.hpp"
#include "src/utils/scheduler/Scheduler.hpp"
#include "src/drivers/logger/Logger.hpp"

using namespace drivers::logger;

namespace drivers::watermeter::kamstrup::transport {

CommandQueue::CommandQueue(std::shared_ptr<IApplicationLayer> appLayer,
                           const CommandQueueConfig& cfg)
    : appLayer_(appLayer)
    , config_(cfg)
{
    auto timerFactory = std::make_shared<drivers::timer::TimerFactory>();

    timeoutTimer_ = timerFactory->getTimer();
    timeoutTimer_->setupInterruptHandler([](void* arg) {
        auto* self = static_cast<CommandQueue*>(arg);
        utils::Scheduler::schedule([self](void*) {
            self->onTimeout();
        });
    }, this);

    settleTimer_ = timerFactory->getTimer();
    settleTimer_->setupInterruptHandler([](void* arg) {
        auto* self = static_cast<CommandQueue*>(arg);
        utils::Scheduler::schedule([self](void*) {
            self->onSettled();
        });
    }, this);
}

void CommandQueue::enqueue(std::shared_ptr<ICommand> cmd)
{
    if (!cmd) {
        return;
    }
    queue_.push(cmd);
    if (!busy_) {
        if (wakeupDriver_ && !wakeupDriver_->isAwake()) {
            wakeupDriver_->wakeup();
            settleTimer_->start(config_.settleDelayUs, drivers::timer::TimerMode::SINGLE_SHOT);
        } else {
            sendNext();
        }
    }
}

void CommandQueue::sendNext()
{
    if (queue_.empty() || busy_) {
        return;
    }
    busy_ = true;
    currentCmd_ = queue_.front();
    currentSeq_ = ++seq_;
    timeoutSeq_ = currentSeq_;

    uint8_t cid = currentCmd_->getCid();
    if (appLayer_) {
        appLayer_->registerHandler(cid, [this, seq = currentSeq_](const std::vector<uint8_t>& payload) {
            if (seq == currentSeq_) {
                currentCmd_->onExecuteResult(ExecuteResult::SUCCESS, payload);
                this->onCommandDone();
            }
        });
    }
    logInfo("CommandQueue: sending CID 0x%02X", cid);
    currentCmd_->execute();
    timeoutTimer_->start(config_.commandTimeoutUs, drivers::timer::TimerMode::SINGLE_SHOT);
}

void CommandQueue::onCommandDone()
{
    if (!busy_) {
        return;
    }
    timeoutTimer_->stop();
    if (!queue_.empty()) {
        queue_.pop();
    }
    currentCmd_.reset();
    busy_ = false;
    if (!queue_.empty()) {
        sendNext();
    } else {
        if (wakeupDriver_ && wakeupDriver_->isAwake()) {
            wakeupDriver_->sleep();
        }
    }
}

void CommandQueue::onTimeout()
{
    if (!busy_) {
        return;
    }
    if (timeoutSeq_ != currentSeq_) {
        return;
    }
    timeoutTimer_->stop();

    ++currentSeq_;

    if (currentCmd_) {
        currentCmd_->onExecuteResult(ExecuteResult::TIMEOUT, {});
    }

    if (!queue_.empty()) {
        queue_.pop();
    }
    currentCmd_.reset();
    busy_ = false;

    if (!queue_.empty()) {
        sendNext();
    } else {
        if (wakeupDriver_ && wakeupDriver_->isAwake()) {
            wakeupDriver_->sleep();
        }
    }
}

void CommandQueue::setWakeupDriver(std::shared_ptr<drivers::watermeter::wakeup::IWatermeterWakeupDriver> driver)
{
    wakeupDriver_ = driver;
}

void CommandQueue::onSettled()
{
    sendNext();
}

} // namespace drivers::watermeter::kamstrup::transport
