#include "SimpleScheduler.hpp"
#include <Arduino.h>
#include "src/drivers/logger/Logger.hpp"

using namespace utils;
using namespace drivers::logger;

SimpleScheduler::SimpleScheduler()
{
}

void SimpleScheduler::process()
{
    bool empty;
    do {
        noInterrupts();
        empty = queue.empty();
        std::function<void(void*)> work;
        if (!empty) {
            work = queue.front();
            queue.pop();
        }
        interrupts();

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
    noInterrupts();
    bool isSpaceInQueue = (queue.size() < MAX_QUEUE_SIZE);
    if (work && isSpaceInQueue) {
        queue.push(work);
    } else if (work) {
        dropped_count++;
    }
    interrupts();
}
