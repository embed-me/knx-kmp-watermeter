#include <memory>

#include "drivers/ArduinoDriverFactory.hpp"

#include "drivers/logger/Logger.hpp"
#include "drivers/logger/ILogger.hpp"

#include "drivers/gpio/IGpioDriver.hpp"

#include "drivers/knx/IKnxDriver.hpp"

#include "drivers/watchdog/IWatchdogDriver.hpp"

#include "drivers/uart/IUartDriver.hpp"

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

/* Drivers */
std::shared_ptr<drivers::IDriverFactory> driverFactory = std::make_shared<drivers::ArduinoDriverFactory>();
#ifdef KNX_UP_BUZZER_DISABLE_LOGGING
    std::shared_ptr<drivers::logger::ILogger> logger = nullptr;
#else
    std::shared_ptr<drivers::logger::ILogger> logger = driverFactory->getLoggerDriver();
#endif
std::shared_ptr<drivers::gpio::IGpioDriver> gpio = driverFactory->getGpioDriver();
std::shared_ptr<drivers::knx::IKnxDriver> knx = driverFactory->getKnxDriver();
std::shared_ptr<drivers::watchdog::IWatchdogDriver> watchdog = driverFactory->getWatchdogDriver();
std::shared_ptr<drivers::uart::IUartDriver> kmpUart = driverFactory->getUartDriver();

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
    logger->init(drivers::logger::LOGLEVEL::LOGLEVEL_TRACE, false);

    //watchdog->enable(WDT_TIMEOUT_MS);

    gpio->setConfig(knxProgLed);
    gpio->setConfig(knxProgButton);
    gpio->setupInterruptHandler(knxProgButton, buttonInterrupt);

    bool stackConfigured = knx->init();

    auto& knxConfig = knx->getKnxConfig();

    drivers::uart::UartConfig kmpUartCfg = {
        .baud = 1200,
        .dataBits = 8,
        .parity = 0,
        .stopBits = 2,
        .txPin = 20,
        .rxPin = 21
    };

    watermeterApp = std::make_shared<application::WatermeterApp>();
    watermeterApp->init(kmpUart, kmpUartCfg, knxConfig);
    knx->setProgmodeChangeCallback([](bool val) {
        gpio->writeValue(knxProgLed, val);
    });

    std::string version = knx->getApplicationVersion();
    logInfo("Application Version: " + version);
}

void loop() {
    //watchdog->feed();
    watermeterApp->process();
    scheduler->process();
}

/* Core 1 */
// void setup1() {
// }

// void loop1() {
// }