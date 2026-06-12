#ifndef _ARDUINO_TIMER_HPP_
#define _ARDUINO_TIMER_HPP_

#include "../ITimer.hpp"
#include "pico/time.h"

#include "src/drivers/logger/ILogger.hpp"

#include <memory>

namespace drivers::timer {

class ArduinoTimer : public ITimer
{
public:
    ArduinoTimer();
    ~ArduinoTimer() = default;

    virtual void start(uint32_t timeout_us, enum TimerMode mode) override;
    virtual void stop() override;
    virtual void setupInterruptHandler(std::function<void(void*)> handler, void *arg) override;
private:
    int convertMode(const enum TimerMode &mode);
    static int64_t interruptHandler(alarm_id_t id, void *user_data);
    
    std::function<void(void*)> handler = nullptr;
    void *handlerArg = nullptr;

    enum TimerMode mode = TimerMode::SINGLE_SHOT;
    uint32_t timeout_us = 0;
    alarm_id_t alarm_id = 0;
};

}

#endif //_ARDUINO_TIMER_HPP_