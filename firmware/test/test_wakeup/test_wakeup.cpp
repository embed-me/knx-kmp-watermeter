#include <gtest/gtest.h>
#include "src/drivers/motion/MotionConfig.hpp"
#include "src/drivers/motion/IMotionDriver.hpp"
#include "src/drivers/watermeter/wakeup/IWatermeterWakeupDriver.hpp"
#include "src/drivers/watermeter/wakeup/WatermeterWakeupDriver.hpp"

#include "src/drivers/watermeter/wakeup/WatermeterWakeupDriver.cpp"

#include "../mocks/motion/mock_motion_driver.hpp"

#include <memory>

using namespace drivers::motion;
using namespace drivers::watermeter::wakeup;

class WatermeterWakeupDriverTest : public ::testing::Test {
protected:
    void SetUp() override {
        mockMotion = std::make_shared<MockMotionDriver>();
        cfg = MotionConfig{.pin = 13};
        mockMotion->setConfig(cfg);
        driver = std::make_shared<WatermeterWakeupDriver>(mockMotion, cfg);
    }

    std::shared_ptr<MockMotionDriver> mockMotion;
    MotionConfig cfg;
    std::shared_ptr<WatermeterWakeupDriver> driver;
};

TEST_F(WatermeterWakeupDriverTest, WakeupWrites180) {
    driver->wakeup();
    EXPECT_EQ(mockMotion->lastAngle, 180);
}

TEST_F(WatermeterWakeupDriverTest, SleepWrites0) {
    driver->sleep();
    EXPECT_EQ(mockMotion->lastAngle, 0);
}

TEST_F(WatermeterWakeupDriverTest, WakeupThenSleepAngleSequence) {
    driver->wakeup();
    EXPECT_EQ(mockMotion->lastAngle, 180);
    driver->sleep();
    EXPECT_EQ(mockMotion->lastAngle, 0);
}

TEST_F(WatermeterWakeupDriverTest, MultipleWakeupCalls) {
    driver->wakeup();
    EXPECT_EQ(mockMotion->lastAngle, 180);
    driver->wakeup();
    EXPECT_EQ(mockMotion->lastAngle, 180);
}

TEST_F(WatermeterWakeupDriverTest, IsAwakeFalseInitially) {
    EXPECT_FALSE(driver->isAwake());
}

TEST_F(WatermeterWakeupDriverTest, IsAwakeTrueAfterWakeup) {
    driver->wakeup();
    EXPECT_TRUE(driver->isAwake());
}

TEST_F(WatermeterWakeupDriverTest, IsAwakeFalseAfterSleep) {
    driver->wakeup();
    driver->sleep();
    EXPECT_FALSE(driver->isAwake());
}
