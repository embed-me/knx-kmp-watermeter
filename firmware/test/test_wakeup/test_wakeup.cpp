#include <gtest/gtest.h>
#include "src/drivers/motion/MotionConfig.hpp"
#include "src/drivers/motion/IMotionDriver.hpp"
#include "src/drivers/watermeter/wakeup/IWatermeterWakeupDriver.hpp"
#include "src/drivers/watermeter/wakeup/WatermeterWakeupDriver.hpp"

#include "src/drivers/watermeter/wakeup/WatermeterWakeupDriver.cpp"
#include "src/drivers/logger/Logger.cpp"
#include "src/utils/scheduler/Scheduler.cpp"

#include "src/utils/scheduler/Scheduler.hpp"

#include "../mocks/motion/mock_motion_driver.hpp"
#include "../mocks/timer/mock_timer.hpp"

#include <memory>

using namespace drivers::motion;
using namespace drivers::watermeter::wakeup;

struct ImmediateSchedulerStrategy : public utils::ISchedulerStrategy {
    void process() override {}
    void schedule(std::function<void(void*)> work) override {
        work(nullptr);
    }
};

class WatermeterWakeupDriverTest : public ::testing::Test {
protected:
    void SetUp() override {
        scheduler_ = std::make_unique<utils::Scheduler>(
            std::make_shared<ImmediateSchedulerStrategy>());

        mockMotion = std::make_shared<MockMotionDriver>();
        cfg = MotionConfig{.pin = 13, .wakeupAngle = 180, .sleepAngle = 0, .settleDelayUs = 3000000};
        mockMotion->setConfig(cfg);
        settleTimer = std::make_shared<MockTimer>();
        driver = std::make_shared<WatermeterWakeupDriver>(mockMotion, cfg, settleTimer);
    }

    void TearDown() override {
        driver.reset();
        utils::Scheduler s(nullptr);
    }

    std::unique_ptr<utils::Scheduler> scheduler_;
    std::shared_ptr<MockMotionDriver> mockMotion;
    std::shared_ptr<MockTimer> settleTimer;
    MotionConfig cfg;
    std::shared_ptr<WatermeterWakeupDriver> driver;
};

TEST_F(WatermeterWakeupDriverTest, WakeupWritesAngleAndStartsSettle) {

    bool readyCalled = false;
    driver->wakeup([&]() { readyCalled = true; });

    EXPECT_EQ(mockMotion->lastAngle, 180);
    EXPECT_TRUE(settleTimer->started);
    EXPECT_EQ(settleTimer->lastTimeoutUs, 3000000u);
    EXPECT_FALSE(readyCalled);
}

TEST_F(WatermeterWakeupDriverTest, WakeupCallsOnReadyAfterSettle) {

    bool readyCalled = false;
    driver->wakeup([&]() { readyCalled = true; });

    settleTimer->fire();
    EXPECT_TRUE(readyCalled);
}

TEST_F(WatermeterWakeupDriverTest, SleepWritesAngle) {
    driver->sleep();
    EXPECT_EQ(mockMotion->lastAngle, 0);
}

TEST_F(WatermeterWakeupDriverTest, WakeupThenSleepAngleSequence) {
    driver->wakeup([]() {});
    EXPECT_EQ(mockMotion->lastAngle, 180);
    settleTimer->fire();
    driver->sleep();
    EXPECT_EQ(mockMotion->lastAngle, 0);
}

TEST_F(WatermeterWakeupDriverTest, WakeupWhileAwakeCallsOnReadyImmediately) {

    bool firstReady = false;
    driver->wakeup([&]() { firstReady = true; });
    settleTimer->fire();
    EXPECT_TRUE(firstReady);

    bool secondReady = false;
    driver->wakeup([&]() { secondReady = true; });
    EXPECT_TRUE(secondReady);
    EXPECT_EQ(settleTimer->startCount, 1);
}

TEST_F(WatermeterWakeupDriverTest, WakeupWhileSettlingIsIgnored) {

    bool ready1 = false;
    driver->wakeup([&]() { ready1 = true; });

    bool ready2 = false;
    driver->wakeup([&]() { ready2 = true; });

    settleTimer->fire();
    EXPECT_TRUE(ready1);
    EXPECT_FALSE(ready2);
}
