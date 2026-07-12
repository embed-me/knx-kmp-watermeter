#include "ArduinoDriverFactory.hpp"

#include "timer/arduino/ArduinoTimer.hpp"
#include "gpio/arduino/ArduinoGpioDriver.hpp"
#include "logger/arduino/ArduinoLogger.hpp"
#include "knx/arduino/ThelsingKnxDriver.hpp"
#include "watchdog/arduino/ArduinoWatchdogDriver.hpp"
#include "uart/arduino/ArduinoUartDriver.hpp"
#include "motion/arduino/ArduinoMotionDriver.hpp"
#include "watermeter/wakeup/WatermeterWakeupDriver.hpp"

#include "logger/Logger.hpp"

using namespace drivers;
using namespace drivers::logger;

ArduinoDriverFactory::ArduinoDriverFactory()
{
}

std::shared_ptr<timer::ITimerDriverFactory> ArduinoDriverFactory::getTimerDriverFactory()
{
    logTrace("creating new Timer Factory");
    auto timerFactory = std::make_shared<timer::TimerFactory>();
    return timerFactory;
}

std::shared_ptr<gpio::IGpioDriver> ArduinoDriverFactory::getGpioDriver()
{
    logTrace("creating new Gpio Driver");
    auto gpio = std::make_shared<gpio::ArduinoGpioDriver>();
    return gpio;
}

std::shared_ptr<logger::ILogger> ArduinoDriverFactory::getLoggerDriver()
{
    auto logger = std::make_shared<logger::ArduinoLogger>();
    return logger;
}

std::shared_ptr<knx::IKnxDriver> ArduinoDriverFactory::getKnxDriver()
{
    logTrace("creating new Knx Driver");
    auto knx = std::make_shared<knx::ThelsingKnxDriver>();
    return knx;
}

std::shared_ptr<watchdog::IWatchdogDriver> ArduinoDriverFactory::getWatchdogDriver()
{
    logTrace("creating new Watchdog Driver");
    auto watchdog = std::make_shared<watchdog::ArduinoWatchdogDriver>();
    return watchdog;
}

std::shared_ptr<uart::IUartDriver> ArduinoDriverFactory::getUartDriver()
{
    logTrace("creating new UART Driver");
    auto uartDriver = std::make_shared<uart::ArduinoUartDriver>();
    return uartDriver;
}

std::shared_ptr<motion::IMotionDriver> ArduinoDriverFactory::getMotionDriver()
{
    logTrace("creating new Motion Driver");
    auto motion = std::make_shared<motion::ArduinoMotionDriver>();
    return motion;
}

std::shared_ptr<watermeter::wakeup::IWatermeterWakeupDriver> ArduinoDriverFactory::getWatermeterWakeupDriver(
    std::shared_ptr<motion::IMotionDriver> motion,
    const motion::MotionConfig& cfg)
{
    logTrace("creating new WatermeterWakeup Driver");
    auto settleTimer = getTimer();
    auto wakeup = std::make_shared<watermeter::wakeup::WatermeterWakeupDriver>(motion, cfg, settleTimer);
    return wakeup;
}