#ifndef _I_TIMER_FACTORY_HPP_
#define _I_TIMER_FACTORY_HPP_

#include "ITimer.hpp"
#include "src/drivers/logger/ILogger.hpp"

#include <memory>

namespace drivers::timer {

class TimerFactory
{
public:
    TimerFactory();
    ~TimerFactory() = default;

    std::shared_ptr<ITimer> getTimer();
private:
};

}

#endif //_I_TIMER_FACTORY_HPP_




