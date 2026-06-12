#include "ThelsingKnxDriver.hpp"

#include <sstream>
#include "src/drivers/logger/Logger.hpp"
#include "src/utils/scheduler/Scheduler.hpp"

using namespace drivers::knx;
using namespace drivers::logger;

ThelsingKnxDriver* ThelsingKnxDriver::instance = nullptr;

ThelsingKnxDriver::ThelsingKnxDriver(std::shared_ptr<drivers::ITimerDriverFactory> timerFactory) : 
    stack(KnxFacade<RP2040ArduinoPlatform, Bau07B0>()), 
    config(KnxConfig(stack)),
    timer(timerFactory->getTimer())
{
    instance = this;

    timer->setupInterruptHandler(loop, this);
}

void ThelsingKnxDriver::progLedOnCallback()
{
    if (instance && instance->progmodeCallback) {
        logInfo("Prog Mode On");
        instance->progmodeCallback(true);
    }
}

void ThelsingKnxDriver::progLedOffCallback()
{
    if (instance && instance->progmodeCallback) {
        logInfo("Prog Mode Off");
        instance->progmodeCallback(false);
    }
}

bool ThelsingKnxDriver::init()
{
    bool stackConfigured = true;

    timer->start(1000, drivers::timer::TimerMode::RECURRING);

    stack.setProgLedOnCallback(progLedOnCallback);
    stack.setProgLedOffCallback(progLedOffCallback);

    /* reads:
    adress table
    association table
    groupobject table
    parameters
    */
    stack.readMemory();

    if (stack.individualAddress() == 0) {
        logWarning("Individual KNX Adress is not set!");
        stack.bau().deviceObject().individualAddress(1); //65535
        stackConfigured = false;
    }

    if (!stack.configured()) {
        logWarning("KNX not configured!");
        stackConfigured = false;
    }

    stack.start();

    return stackConfigured;
}

std::string drivers::knx::ThelsingKnxDriver::getApplicationVersion()
{
    char buffer[16];

    struct ApplicationVersion version = config.getApplicationVersion();
    
    std::snprintf(buffer, sizeof(buffer), "%d.%d", version.major, version.minor);
    return std::string(buffer);
}

KnxConfig &ThelsingKnxDriver::getKnxConfig()
{
    return config;
}

void ThelsingKnxDriver::register_cb(int ko_number, std::function<void(GroupObject &)>callback)
{
    stack.getGroupObject(ko_number).callback(callback);
}

void ThelsingKnxDriver::setProgmodeChangeCallback(std::function<void(bool)> callback)
{
    this->progmodeCallback = callback;
}

void ThelsingKnxDriver::toggleProgMode()
{
    stack.toggleProgMode();
    
}

void ThelsingKnxDriver::loop(void *arg)
{
    auto inst = static_cast<ThelsingKnxDriver*>(arg);
    
    utils::Scheduler::schedule([inst](void*) {
        if (inst) {
            inst->stack.loop();
        }
    });
}
