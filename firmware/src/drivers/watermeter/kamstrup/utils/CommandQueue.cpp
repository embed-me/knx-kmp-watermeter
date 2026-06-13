#include "CommandQueue.hpp"
#include "src/utils/scheduler/Scheduler.hpp"
#include "src/drivers/logger/Logger.hpp"

using namespace drivers::logger;

namespace drivers::watermeter::kamstrup::transport {

constexpr uint32_t COMMAND_TIMEOUT_US = 1600000; // 1.6 seconds per MULTICAL spec
constexpr uint32_t RETRY_DELAY_US = 1600000;     // 1.6 seconds after timeout before next request

CommandQueue::CommandQueue()
{
    auto timerFactory = std::make_shared<drivers::timer::TimerFactory>();

    timeoutTimer_ = timerFactory->getTimer();
    timeoutTimer_->setupInterruptHandler([](void* arg) {
        auto* self = static_cast<CommandQueue*>(arg);
        utils::Scheduler::schedule([self](void*) {
            self->onTimeout();
        });
    }, this);

    retryTimer_ = timerFactory->getTimer();
    retryTimer_->setupInterruptHandler([](void* arg) {
        auto* self = static_cast<CommandQueue*>(arg);
        utils::Scheduler::schedule([self](void*) {
            self->onRetryDelayExpired();
        });
    }, this);
}

void CommandQueue::enqueue(std::shared_ptr<ICommand> cmd)
{
    if (!cmd) {
        return;
    }
    queue_.push(cmd);
    if (!busy_ && !waitingRetry_) {
        sendNext();
    }
}

void CommandQueue::sendNext()
{
    if (queue_.empty() || busy_ || waitingRetry_) {
        return;
    }
    busy_ = true;
    currentCmd_ = queue_.front();
    currentSeq_ = ++seq_;
    timeoutSeq_ = currentSeq_;
    currentCmd_->registerListener([this, seq = currentSeq_](const CommandResult&) {
        if (seq == currentSeq_) {
            this->onCommandDone();
        }
    });
    logInfo("CommandQueue: sending CID 0x%02X", currentCmd_->getCid());
    currentCmd_->execute();
    timeoutTimer_->start(COMMAND_TIMEOUT_US, drivers::timer::TimerMode::SINGLE_SHOT);
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
    }
}

void CommandQueue::onTimeout()
{
    if (!busy_) {
        return;
    }
    if (timeoutSeq_ != currentSeq_) {
        return; // stale timeout for a previous command
    }
    timeoutTimer_->stop();

    // Invalidates the queue's internal listener (seq check) so onCommandDone won't fire
    ++currentSeq_;

    CommandResult timeoutResult;
    timeoutResult.result = CommandResult::Result::TIMEOUT;
    if (currentCmd_) {
        currentCmd_->onResult(timeoutResult);
    }

    if (!queue_.empty()) {
        queue_.pop();
    }
    currentCmd_.reset();
    busy_ = false;
    waitingRetry_ = true;
    retryTimer_->stop();
    retryTimer_->start(RETRY_DELAY_US, drivers::timer::TimerMode::SINGLE_SHOT);
}

void CommandQueue::onRetryDelayExpired()
{
    waitingRetry_ = false;
    retryTimer_->stop(); // clear alarm_id so it can be restarted
    if (!queue_.empty()) {
        sendNext();
    }
}

} // namespace drivers::watermeter::kamstrup::transport
