#ifndef _I_TIMER_FACTORY_HPP_
#define _I_TIMER_FACTORY_HPP_

#include "ITimer.hpp"
#include "../ITimerDriverFactory.hpp"
#include "src/drivers/logger/ILogger.hpp"

#include <memory>

namespace drivers::timer {

class TimerFactory : public ITimerDriverFactory
{
public:
    TimerFactory();
    virtual ~TimerFactory() = default;

    virtual std::shared_ptr<ITimer> getTimer() override;
private:
};

}

#endif //_I_TIMER_FACTORY_HPP_




