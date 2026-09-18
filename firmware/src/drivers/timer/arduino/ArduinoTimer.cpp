#include "ArduinoTimer.hpp"
#include "pico/stdlib.h"
#include "hardware/timer.h"

#include "src/utils/scheduler/Scheduler.hpp"
#include "src/drivers/logger/Logger.hpp"

using namespace drivers::timer;
using namespace drivers::logger;

ArduinoTimer::ArduinoTimer()
{
}

int64_t ArduinoTimer::interruptHandler(alarm_id_t id, void *user_data) 
{
    ArduinoTimer *instance = static_cast<ArduinoTimer*>(user_data);

    if (!instance) {
        return 0;
    }
    
    if (id != instance->alarm_id) {
        return 0;
    }

    if (instance->handler) {
        instance->handler(instance->handlerArg);
    }

    int64_t result = instance->convertMode(instance->mode);
    if (instance->mode == TimerMode::SINGLE_SHOT) {
        instance->alarm_id = 0;
    }
    return result;
}

int ArduinoTimer::convertMode(const enum TimerMode &mode)
{
    int val = 0;

    switch(mode) {
        case TimerMode::SINGLE_SHOT:
            val = 0;
            break;
        case TimerMode::RECURRING:
            val = -1 * (int)this->timeout_us;
            break;
        default:
            logWarning("invalid Mode %d", mode);
            break;
    }

    return val;
}

void ArduinoTimer::start(uint32_t timeout_us, enum TimerMode mode)
{
    if (alarm_id != 0) {
        logError("timer already started, abort");
        return;
    }

    this->mode = mode;
    this->timeout_us = timeout_us;
    
    logTrace("starting timer with %d us, mode %d", timeout_us, mode);
    
    alarm_id = add_alarm_in_us(timeout_us, interruptHandler, this, false);
    if (alarm_id <= 0) {
        logError("starting timer failed with error %d", alarm_id);
        stop();
    }
}

void ArduinoTimer::stop()
{
    if (alarm_id != 0) {
        logTrace("stopping timer");
        cancel_alarm(alarm_id);
        alarm_id = 0;
    }
}

void ArduinoTimer::setupInterruptHandler(std::function<void(void *)> handler, void *arg)
{
    this->handler = handler;
    this->handlerArg = arg;
}