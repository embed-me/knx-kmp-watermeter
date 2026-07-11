#ifndef _THELSING_KNX_DRIVER_HPP_
#define _THELSING_KNX_DRIVER_HPP_

#include "../IKnxDriver.hpp"
#include "src/drivers/ITimerDriverFactory.hpp"
#include "knx_facade.h"

namespace drivers::knx {

class ThelsingKnxDriver : public IKnxDriver
{
public:
    explicit ThelsingKnxDriver();
    ~ThelsingKnxDriver() = default;

    virtual bool init() override;
    std::string getApplicationVersion() override;
    virtual KnxConfig &getKnxConfig() override;
    virtual void register_cb(int ko_number, std::function<void(GroupObject&)>) override;
    virtual void setProgmodeChangeCallback(std::function<void(bool)> callback) override;
    virtual void toggleProgMode() override;
    virtual void loop() override;
private:
    static void loop(void *arg);
    static void progLedOnCallback();
    static void progLedOffCallback();

    static ThelsingKnxDriver *instance;
    KnxFacade<RP2040ArduinoPlatform, Bau07B0> stack;
    KnxConfig config;

    std::function<void(bool)> progmodeCallback = nullptr;
};

}

#endif //_THELSING_KNX_DRIVER_HPP_