#ifndef _ARDUINO_DRIVER_FACTORY_HPP_
#define _ARDUINO_DRIVER_FACTORY_HPP_

#include "IDriverFactory.hpp"

#include "timer/ITimer.hpp"
#include "gpio/IGpioDriver.hpp"
#include "watchdog/IWatchdogDriver.hpp"
#include "uart/IUartDriver.hpp"

#include "logger/ILogger.hpp"

#include <memory>

namespace drivers {

class ArduinoDriverFactory : public IDriverFactory,
                             public std::enable_shared_from_this<ArduinoDriverFactory>
{
public:
    ArduinoDriverFactory();
    ~ArduinoDriverFactory() = default;

    virtual std::shared_ptr<timer::ITimer> getTimer() override;
    virtual std::shared_ptr<gpio::IGpioDriver> getGpioDriver() override;
    virtual std::shared_ptr<logger::ILogger> getLoggerDriver() override;
    virtual std::shared_ptr<knx::IKnxDriver> getKnxDriver() override;
    virtual std::shared_ptr<watchdog::IWatchdogDriver> getWatchdogDriver() override;
    virtual std::shared_ptr<uart::IUartDriver> getUartDriver() override;
    virtual std::shared_ptr<motion::IMotionDriver> getMotionDriver() override;
    virtual std::shared_ptr<watermeter::wakeup::IWatermeterWakeupDriver> getWatermeterWakeupDriver(
        std::shared_ptr<motion::IMotionDriver> motion,
        const motion::MotionConfig& cfg) override;
private:
};

}

#endif //_ARDUINO_DRIVER_FACTORY_HPP_




