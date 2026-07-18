#ifndef _MOCK_WAKEUP_DRIVER_HPP_
#define _MOCK_WAKEUP_DRIVER_HPP_

#include "src/drivers/watermeter/wakeup/IWatermeterWakeupDriver.hpp"

struct MockWakeupDriver : public drivers::watermeter::wakeup::IWatermeterWakeupDriver {
    bool awake = false;
    int wakeupCount = 0;
    int sleepCount = 0;
    std::function<void()> lastOnReady;
    uint32_t settleDelayUs = 3000000;

    void wakeup(std::function<void()> onReady) override {
        awake = true;
        wakeupCount++;
        lastOnReady = onReady;
    }

    void sleep() override {
        awake = false;
        sleepCount++;
    }

    uint32_t getSettleDelayUs() const override {
        return settleDelayUs;
    }

    void fireReady() {
        if (lastOnReady) {
            auto cb = lastOnReady;
            lastOnReady = nullptr;
            cb();
        }
    }

    void reset() {
        awake = false;
        wakeupCount = 0;
        sleepCount = 0;
        lastOnReady = nullptr;
    }
};

#endif //_MOCK_WAKEUP_DRIVER_HPP_
