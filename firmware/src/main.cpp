#include <memory>

#include "drivers/ArduinoDriverFactory.hpp"
#include "drivers/motion/MotionConfig.hpp"

#include "drivers/logger/Logger.hpp"
#include "drivers/logger/ILogger.hpp"

#include "drivers/gpio/IGpioDriver.hpp"

#include "drivers/knx/IKnxDriver.hpp"

#include "drivers/watchdog/IWatchdogDriver.hpp"

#include "drivers/uart/IUartDriver.hpp"

#include "drivers/watermeter/kamstrup/utils/CommandQueueConfig.hpp"

#include "utils/scheduler/Scheduler.hpp"
#include "utils/scheduler/strategy/ISchedulerStrategy.hpp"
#include "utils/scheduler/strategy/simple/SimpleScheduler.hpp"

#include "application/WatermeterApp.hpp"

/* GPIOs */
struct drivers::gpio::GpioConfig knxProgButton = {
    .pin = 7,
    .sensitivity = drivers::gpio::GPIO_SENSITIVITY::INTERRUPT_CHANGE,
    .direction = drivers::gpio::GPIO_DIRECTION::DIRECTION_INPUT,
    .pull = drivers::gpio::GPIO_PULL::PULL_HIGH
};

struct drivers::gpio::GpioConfig knxProgLed = {
    .pin = 6,
    .sensitivity = drivers::gpio::GPIO_SENSITIVITY::ACTIVE_HIGH,
    .direction = drivers::gpio::GPIO_DIRECTION::DIRECTION_OUTPUT,
    .pull = drivers::gpio::GPIO_PULL::PULL_NONE
};

struct drivers::uart::UartConfig kmpUartCfg = {
    .baud = 1200,
    .dataBits = 8,
    .parity = 0,
    .stopBits = 2,
    .txPin = 20,
    .rxPin = 21
};

drivers::motion::MotionConfig wakeupMotionCfg = { 
    .pin = 13,
    .minPulseWidth = 500,
    .maxPulseWidth = 2500,
    .wakeupAngle = 90, 
    .sleepAngle = 50,
    .settleDelayUs = 5000000
};

drivers::watermeter::kamstrup::transport::CommandQueueConfig kmpQueueCfg = {
    .commandTimeoutUs = 1600000
};

std::shared_ptr<drivers::IDriverFactory> driverFactory =
    std::make_shared<drivers::ArduinoDriverFactory>();
#ifdef KNX_KMP_WATERMETER_DISABLE_LOGGING
    std::shared_ptr<drivers::logger::ILogger> logger = nullptr;
#else
    std::shared_ptr<drivers::logger::ILogger> logger = driverFactory->getLoggerDriver();
#endif
std::shared_ptr<drivers::gpio::IGpioDriver> gpio = driverFactory->getGpioDriver();
std::shared_ptr<drivers::knx::IKnxDriver> knx = driverFactory->getKnxDriver();
std::shared_ptr<drivers::watchdog::IWatchdogDriver> watchdog = driverFactory->getWatchdogDriver();
std::shared_ptr<drivers::uart::IUartDriver> kmpUart = driverFactory->getUartDriver();
std::shared_ptr<drivers::timer::ITimerDriverFactory> timerFactory = driverFactory->getTimerDriverFactory();

/* Utilities */
std::shared_ptr<utils::ISchedulerStrategy> schedulerStrategy = std::make_shared<utils::SimpleScheduler>();
std::shared_ptr<utils::Scheduler> scheduler = std::make_shared<utils::Scheduler>(schedulerStrategy);

#define WDT_TIMEOUT_MS 8000
#define PROG_BTN_PRESS_MIN_MILLIS 50
#define PROG_BTN_PRESS_MAX_MILLIS 500

void buttonPressHandler(void *arg) 
{
    static uint32_t lastEvent = 0;
    static uint32_t lastPressed = 0;

    uint32_t diff = millis() - lastEvent;

    if (diff >= PROG_BTN_PRESS_MIN_MILLIS && diff <= PROG_BTN_PRESS_MAX_MILLIS)
    {
        if (millis() - lastPressed > 200)
        {
            knx->toggleProgMode();
            lastPressed = millis();
        }
    }

    lastEvent = millis();
}

void buttonInterrupt()
{
    utils::Scheduler::schedule(buttonPressHandler);
}

std::shared_ptr<application::WatermeterApp> watermeterApp = nullptr;

/* Core 0 */
void setup() {
    drivers::logger::Logger::setLogger(logger);
    drivers::logger::Logger::getLogger()->init(drivers::logger::LOGLEVEL::LOGLEVEL_TRACE, true);

    if (watchdog->wasRebootedByWatchdog()) {
        logWarning("System rebooted by watchdog");
    }

    watchdog->enable(WDT_TIMEOUT_MS);

    gpio->setConfig(knxProgLed);
    gpio->setConfig(knxProgButton);
    gpio->setupInterruptHandler(knxProgButton, buttonInterrupt);
    knx->setProgmodeChangeCallback([](bool val) {
        gpio->writeValue(knxProgLed, val);
    });

    bool stackConfigured = knx->init();

    if (stackConfigured) {
        std::string version = knx->getApplicationVersion();
        logInfo("Application Version: " + version);

        auto& knxConfig = knx->getKnxConfig();
        watermeterApp = std::make_shared<application::WatermeterApp>();

        auto motion = driverFactory->getMotionDriver();
        motion->setConfig(wakeupMotionCfg);
        auto wakeupDriver = driverFactory->getWatermeterWakeupDriver(motion, wakeupMotionCfg);

        watermeterApp->init(kmpUart, kmpUartCfg, knxConfig, kmpQueueCfg, wakeupDriver, timerFactory);
    } else {
        logWarning("KNX Stack initialization failed");
    }
}

void loop() {
    watchdog->feed();
    watermeterApp->process();
    scheduler->process();
    knx->loop();
}

/* Core 1 */
// void setup1() {
// }

// void loop1() {
// }