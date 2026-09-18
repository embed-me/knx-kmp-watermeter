#include "SimpleScheduler.hpp"

using namespace utils;

SimpleScheduler::SimpleScheduler()
{
}

void SimpleScheduler::process()
{
    uint8_t processed = 0;
    bool isMaxPerCycleLimitReached = false;
    
    while (!queue.empty() && !isMaxPerCycleLimitReached) {
        std::function<void(void*)> work = queue.front();
        if (work) {
            work(nullptr);
        }
        queue.pop();
        
        isMaxPerCycleLimitReached = ((processed++) >= MAX_PER_CYCLE_LIMIT);
    }
}

void SimpleScheduler::schedule(std::function<void(void*)> work)
{
    bool isSpaceInQueue = (queue.size() < MAX_QUEUE_SIZE);
    if (work && isSpaceInQueue) {
        queue.push(work);
    }
}
