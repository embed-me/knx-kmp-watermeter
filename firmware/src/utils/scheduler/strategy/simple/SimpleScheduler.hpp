#ifndef _SIMPLE_SCHEDULER_HPP_
#define _SIMPLE_SCHEDULER_HPP_

#include "../ISchedulerStrategy.hpp"
#include <pico/critical_section.h>

#include <queue>
#include <functional>
#include <stdint.h>

namespace utils {

class SimpleScheduler : public ISchedulerStrategy
{
public:
    SimpleScheduler();
    ~SimpleScheduler() = default;

    virtual void process() override;
    virtual void schedule(std::function<void(void*)> work) override;

private:
    static const uint16_t MAX_QUEUE_SIZE = 500;
    std::queue<std::function<void(void*)>> queue;
    uint32_t dropped_count = 0;
    critical_section_t _cs;
};

}

#endif //_SIMPLE_SCHEDULER_HPP_