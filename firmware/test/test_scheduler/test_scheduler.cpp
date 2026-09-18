#include <gtest/gtest.h>
#include "src/utils/scheduler/Scheduler.hpp"
#include "src/utils/scheduler/strategy/ISchedulerStrategy.hpp"

#include "src/utils/scheduler/Scheduler.cpp"

struct MockSchedulerStrategy : public utils::ISchedulerStrategy {
    std::function<void(void*)> lastScheduled;
    bool processCalled = false;

    void process() override { processCalled = true; }
    void schedule(std::function<void(void*)> work) override { lastScheduled = work; }
};

class SchedulerTest : public ::testing::Test {
protected:
    void TearDown() override {
        utils::Scheduler s(nullptr);
    }
};

TEST_F(SchedulerTest, ScheduleDelegatesToStrategy) {
    auto mock = std::make_shared<MockSchedulerStrategy>();
    utils::Scheduler s(mock);

    auto task = [](void*) {};
    utils::Scheduler::schedule(task);

    EXPECT_TRUE(mock->lastScheduled != nullptr);
}

TEST_F(SchedulerTest, ProcessDelegatesToStrategy) {
    auto mock = std::make_shared<MockSchedulerStrategy>();
    utils::Scheduler s(mock);

    utils::Scheduler::process();

    EXPECT_TRUE(mock->processCalled);
}

TEST_F(SchedulerTest, ScheduleWithNullStrategy_NoOpNoCrash) {
    utils::Scheduler s(nullptr);
    EXPECT_NO_THROW(utils::Scheduler::schedule([](void*) {}));
}

TEST_F(SchedulerTest, ProcessWithNullStrategy_NoOpNoCrash) {
    utils::Scheduler s(nullptr);
    EXPECT_NO_THROW(utils::Scheduler::process());
}
