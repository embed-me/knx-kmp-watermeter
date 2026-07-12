#include <gtest/gtest.h>

#define private public
#include "src/drivers/timer/arduino/ArduinoTimer.hpp"
#undef private

#include "src/drivers/timer/arduino/ArduinoTimer.cpp"
#include "src/drivers/logger/Logger.cpp"

#include <memory>

using namespace drivers::timer;

class ArduinoTimerTest : public ::testing::Test {
protected:
    void SetUp() override {
        timer = std::make_shared<ArduinoTimer>();
    }

    std::shared_ptr<ArduinoTimer> timer;
};

TEST_F(ArduinoTimerTest, StartStopStart) {
    timer->start(1000000, TimerMode::SINGLE_SHOT);
    EXPECT_NE(timer->alarm_id, 0);

    timer->stop();
    EXPECT_EQ(timer->alarm_id, 0);

    timer->start(1000000, TimerMode::SINGLE_SHOT);
    EXPECT_NE(timer->alarm_id, 0);
}

TEST_F(ArduinoTimerTest, InterruptHandlerClearsAlarmIdForSingleShot) {
    timer->start(1000000, TimerMode::SINGLE_SHOT);
    alarm_id_t fired_alarm_id = timer->alarm_id;
    EXPECT_NE(fired_alarm_id, 0);

    ArduinoTimer::interruptHandler(fired_alarm_id, timer.get());

    EXPECT_EQ(timer->alarm_id, 0);
}

TEST_F(ArduinoTimerTest, InterruptHandlerDoesNotClearAlarmIdForRecurring) {
    timer->start(1000000, TimerMode::RECURRING);
    alarm_id_t fired_alarm_id = timer->alarm_id;
    EXPECT_NE(fired_alarm_id, 0);

    ArduinoTimer::interruptHandler(fired_alarm_id, timer.get());

    EXPECT_NE(timer->alarm_id, 0);
}

TEST_F(ArduinoTimerTest, CanStartAgainAfterInterruptHandler) {
    timer->start(1000000, TimerMode::SINGLE_SHOT);
    alarm_id_t fired_alarm_id = timer->alarm_id;
    ArduinoTimer::interruptHandler(fired_alarm_id, timer.get());

    EXPECT_EQ(timer->alarm_id, 0);

    timer->start(1000000, TimerMode::SINGLE_SHOT);
    EXPECT_NE(timer->alarm_id, 0);
}
