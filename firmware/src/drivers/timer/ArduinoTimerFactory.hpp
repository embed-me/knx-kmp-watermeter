#ifndef _ARDUINO_TIMER_FACTORY_HPP_
#define _ARDUINO_TIMER_FACTORY_HPP_

#include "ITimer.hpp"
#include "../ITimerDriverFactory.hpp"
#include "src/drivers/logger/ILogger.hpp"

#include <memory>

namespace drivers::timer {

class ArduinoTimerFactory : public ITimerDriverFactory
{
public:
    ArduinoTimerFactory();
    virtual ~ArduinoTimerFactory() = default;

    virtual std::shared_ptr<ITimer> getTimer() override;
private:
};

}

#endif //_ARDUINO_TIMER_FACTORY_HPP_




