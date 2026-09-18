#ifndef _I_TIMER_HPP_
#define _I_TIMER_HPP_

#include <stdint.h>
#include <functional>

namespace drivers::timer {

enum TimerMode {
    SINGLE_SHOT = 0,
    RECURRING = 1
};

class ITimer {
public:
    virtual ~ITimer() = default;

    virtual void setupInterruptHandler(std::function<void(void*)> handler, void *arg) = 0;
    virtual void start(uint32_t timeout_us, enum TimerMode mode) = 0;
    virtual void stop() = 0;
};

}

#endif //_I_TIMER_HPP_