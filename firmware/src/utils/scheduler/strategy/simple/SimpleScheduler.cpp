#include "SimpleScheduler.hpp"
#include "src/drivers/logger/Logger.hpp"

using namespace utils;
using namespace drivers::logger;

SimpleScheduler::SimpleScheduler()
{
    critical_section_init(&_cs);
}

void SimpleScheduler::process()
{
    bool empty;
    do {
        critical_section_enter_blocking(&_cs);
        empty = queue.empty();
        std::function<void(void*)> work;
        if (!empty) {
            work = queue.front();
            queue.pop();
        }
        critical_section_exit(&_cs);

        if (!empty && work) {
            work(nullptr);
        }
    } while (!empty);
    
    if (dropped_count > 0) {
        logWarning("Scheduler dropped %u tasks (queue full)", dropped_count);
        dropped_count = 0;
    }
}

void SimpleScheduler::schedule(std::function<void(void*)> work)
{
    critical_section_enter_blocking(&_cs);
    bool isSpaceInQueue = (queue.size() < MAX_QUEUE_SIZE);
    if (work && isSpaceInQueue) {
        queue.push(work);
    } else if (work) {
        dropped_count++;
    }
    critical_section_exit(&_cs);
}
