#ifndef _COMMAND_QUEUE_HPP_
#define _COMMAND_QUEUE_HPP_

#include "../commands/ICommand.hpp"
#include "src/drivers/timer/TimerFactory.hpp"
#include "src/drivers/timer/ITimer.hpp"

#include <queue>
#include <memory>
#include <functional>
#include <cstdint>

namespace drivers::watermeter::kamstrup::transport {

class CommandQueue {
public:
    CommandQueue();
    ~CommandQueue() = default;

    void enqueue(std::shared_ptr<ICommand> cmd);

private:
    void sendNext();
    void onCommandDone();
    void onTimeout();
    void onRetryDelayExpired();

    std::queue<std::shared_ptr<ICommand>> queue_;
    bool busy_ = false;
    bool waitingRetry_ = false;
    uint32_t seq_ = 0;
    uint32_t currentSeq_ = 0;
    uint32_t timeoutSeq_ = 0;
    std::shared_ptr<drivers::timer::ITimer> timeoutTimer_;
    std::shared_ptr<drivers::timer::ITimer> retryTimer_;
};

}

#endif // _COMMAND_QUEUE_HPP_
