#ifndef _I_SCHEDULER_STRATEGY_HPP_
#define _I_SCHEDULER_STRATEGY_HPP_

#include <functional>

namespace utils {

class ISchedulerStrategy
{
public:
    ~ISchedulerStrategy() = default;

    virtual void process() = 0;
    virtual void schedule(std::function<void(void*)>) = 0;
};

}

#endif //_I_SCHEDULER_STRATEGY_HPP_