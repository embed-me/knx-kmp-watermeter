#ifndef _SCHEDULER_HPP_
#define _SCHEDULER_HPP_

#include "Scheduler.hpp"
#include "strategy/ISchedulerStrategy.hpp"

#include <memory>
#include <functional>

namespace utils {

class Scheduler
{
public:
    explicit Scheduler(std::shared_ptr<ISchedulerStrategy> strategy);
    ~Scheduler() = default;

    static void process();
    static void schedule(std::function<void(void*)>);

private:
    static std::shared_ptr<ISchedulerStrategy> _strategy;
};

}

#endif //_SCHEDULER_HPP_