#ifndef _I_TIMER_DRIVER_FACTORY_HPP_
#define _I_TIMER_DRIVER_FACTORY_HPP_

#include "timer/ITimer.hpp"

#include <memory>

namespace drivers {

class ITimerDriverFactory
{
public:
    ~ITimerDriverFactory() = default;

    virtual std::shared_ptr<timer::ITimer> getTimer() = 0;
};

}

#endif //_I_TIMER_DRIVER_FACTORY_HPP_




