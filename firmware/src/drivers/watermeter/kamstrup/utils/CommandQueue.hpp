#ifndef _COMMAND_QUEUE_HPP_
#define _COMMAND_QUEUE_HPP_

#include "../commands/ICommand.hpp"
#include "../transport/layers/IApplicationLayer.hpp"
#include "CommandQueueConfig.hpp"
#include "src/drivers/timer/TimerFactory.hpp"
#include "src/drivers/timer/ITimer.hpp"
#include "src/drivers/watermeter/wakeup/IWatermeterWakeupDriver.hpp"

#include <queue>
#include <memory>
#include <functional>
#include <cstdint>

namespace drivers::watermeter::kamstrup::transport {

enum class ExecuteResult : uint8_t {
    SUCCESS = 0,
    TIMEOUT = 1
};

class CommandQueue {
public:
    CommandQueue(std::shared_ptr<IApplicationLayer> appLayer,
                 const CommandQueueConfig& cfg = CommandQueueConfig{});
    ~CommandQueue() = default;

    void enqueue(std::shared_ptr<ICommand> cmd);
    void setWakeupDriver(std::shared_ptr<drivers::watermeter::wakeup::IWatermeterWakeupDriver> driver);

private:
    void sendNext();
    void onCommandDone();
    void onTimeout();
    void onSettled();

    std::shared_ptr<IApplicationLayer> appLayer_;
    std::queue<std::shared_ptr<ICommand>> queue_;
    std::shared_ptr<ICommand> currentCmd_;
    bool busy_ = false;
    uint32_t seq_ = 0;
    uint32_t currentSeq_ = 0;
    uint32_t timeoutSeq_ = 0;
    std::shared_ptr<drivers::timer::ITimer> timeoutTimer_;
    std::shared_ptr<drivers::timer::ITimer> settleTimer_;
    std::shared_ptr<drivers::watermeter::wakeup::IWatermeterWakeupDriver> wakeupDriver_;
    CommandQueueConfig config_;
};

}

#endif // _COMMAND_QUEUE_HPP_
