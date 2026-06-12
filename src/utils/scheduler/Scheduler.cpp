#include "Scheduler.hpp"


using namespace utils;


std::shared_ptr<ISchedulerStrategy> Scheduler::_strategy = nullptr;


Scheduler::Scheduler(std::shared_ptr<ISchedulerStrategy> strategy)
{
    _strategy = strategy;
}

void Scheduler::process()
{
    if (_strategy) {
        _strategy->process();
    }
}

void Scheduler::schedule(std::function<void(void*)> task)
{
    if (_strategy) {
        _strategy->schedule(task);
    }
}
