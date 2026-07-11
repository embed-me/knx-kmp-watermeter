#ifndef _I_DRIVER_FACTORY_HPP_
#define _I_DRIVER_FACTORY_HPP_

#include "ITimerDriverFactory.hpp"
#include "IGpioDriverFactory.hpp"
#include "IKnxDriverFactory.hpp"
#include "IMotionDriverFactory.hpp"
#include "IWatermeterWakeupDriverFactory.hpp"
#include "logger/ILogger.hpp"
#include "watchdog/IWatchdogDriver.hpp"
#include "uart/IUartDriver.hpp"

namespace drivers {

class IDriverFactory : public ITimerDriverFactory,
                       public IGpioDriverFactory,
                       public IKnxDriverFactory,
                       public IMotionDriverFactory,
                       public IWatermeterWakeupDriverFactory
{
public:
    ~IDriverFactory() = default;

    virtual std::shared_ptr<logger::ILogger> getLoggerDriver() = 0;
    virtual std::shared_ptr<watchdog::IWatchdogDriver> getWatchdogDriver() = 0;
    virtual std::shared_ptr<uart::IUartDriver> getUartDriver() = 0;
};

}

#endif //_I_DRIVER_FACTORY_HPP_




