#ifndef _MOCK_TIMER_HPP_
#define _MOCK_TIMER_HPP_

#include "src/drivers/timer/ITimer.hpp"

#include <functional>
#include <cstdint>

struct MockTimer : public drivers::timer::ITimer {
    bool started = false;
    bool stopped = false;
    uint32_t lastTimeoutUs = 0;
    drivers::timer::TimerMode lastMode = drivers::timer::TimerMode::SINGLE_SHOT;
    std::function<void(void*)> handler;
    void* handlerArg = nullptr;
    int startCount = 0;
    int stopCount = 0;

    void start(uint32_t timeout_us, drivers::timer::TimerMode mode) override {
        started = true;
        stopped = false;
        lastTimeoutUs = timeout_us;
        lastMode = mode;
        startCount++;
    }

    void stop() override {
        started = false;
        stopped = true;
        stopCount++;
    }

    void setupInterruptHandler(std::function<void(void*)> h, void* arg) override {
        handler = h;
        handlerArg = arg;
    }

    void fire() {
        if (handler) {
            handler(handlerArg);
        }
    }

    void reset() {
        started = false;
        stopped = false;
        startCount = 0;
        stopCount = 0;
    }
};

#endif //_MOCK_TIMER_HPP_
