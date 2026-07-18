#include <gtest/gtest.h>
#include "src/utils/cron/CronMatcher.hpp"
#include "src/utils/cron/CronMatcher.cpp"

using namespace utils::cron;
using namespace drivers::rtc;

class CronMatcherTest : public ::testing::Test {};

TEST_F(CronMatcherTest, StarEveryMinute) {
    CronMatcher cm("* * * * *");
    DateTime dt = {2024, 1, 15, 10, 30, 0};
    EXPECT_TRUE(cm.matches(dt));
}

TEST_F(CronMatcherTest, ExactMinute) {
    CronMatcher cm("30 * * * *");
    DateTime dt = {2024, 1, 15, 10, 30, 0};
    EXPECT_TRUE(cm.matches(dt));
}

TEST_F(CronMatcherTest, ExactMinuteNoMatch) {
    CronMatcher cm("0 * * * *");
    DateTime dt = {2024, 1, 15, 10, 30, 0};
    EXPECT_FALSE(cm.matches(dt));
}

TEST_F(CronMatcherTest, ExactHourAndMinute) {
    CronMatcher cm("30 10 * * *");
    DateTime dt = {2024, 1, 15, 10, 30, 0};
    EXPECT_TRUE(cm.matches(dt));
}

TEST_F(CronMatcherTest, StarSlashEvery15Minutes) {
    CronMatcher cm("*/15 * * * *");
    DateTime dt = {2024, 1, 15, 10, 0, 0};
    EXPECT_TRUE(cm.matches(dt));
    DateTime dt2 = {2024, 1, 15, 10, 15, 0};
    EXPECT_TRUE(cm.matches(dt2));
    DateTime dt3 = {2024, 1, 15, 10, 30, 0};
    EXPECT_TRUE(cm.matches(dt3));
    DateTime dt4 = {2024, 1, 15, 10, 45, 0};
    EXPECT_TRUE(cm.matches(dt4));
    DateTime dt5 = {2024, 1, 15, 10, 7, 0};
    EXPECT_FALSE(cm.matches(dt5));
}

TEST_F(CronMatcherTest, RangeHours) {
    CronMatcher cm("0 9-17 * * *");
    DateTime dt = {2024, 1, 15, 9, 0, 0};
    EXPECT_TRUE(cm.matches(dt));
    DateTime dt2 = {2024, 1, 15, 17, 0, 0};
    EXPECT_TRUE(cm.matches(dt2));
    DateTime dt3 = {2024, 1, 15, 8, 0, 0};
    EXPECT_FALSE(cm.matches(dt3));
    DateTime dt4 = {2024, 1, 15, 18, 0, 0};
    EXPECT_FALSE(cm.matches(dt4));
}

TEST_F(CronMatcherTest, ListMinutes) {
    CronMatcher cm("5,10,15 * * * *");
    DateTime dt = {2024, 1, 15, 10, 5, 0};
    EXPECT_TRUE(cm.matches(dt));
    DateTime dt2 = {2024, 1, 15, 10, 10, 0};
    EXPECT_TRUE(cm.matches(dt2));
    DateTime dt3 = {2024, 1, 15, 10, 12, 0};
    EXPECT_FALSE(cm.matches(dt3));
}

TEST_F(CronMatcherTest, SpecificMonth) {
    CronMatcher cm("0 0 1 6 *");
    DateTime dt = {2024, 6, 1, 0, 0, 0};
    EXPECT_TRUE(cm.matches(dt));
    DateTime dt2 = {2024, 5, 1, 0, 0, 0};
    EXPECT_FALSE(cm.matches(dt2));
}

TEST_F(CronMatcherTest, InvalidExpressionReturnsNoMatch) {
    CronMatcher cm("");
    DateTime dt = {2024, 1, 15, 10, 30, 0};
    EXPECT_FALSE(cm.matches(dt));
}

TEST_F(CronMatcherTest, EveryTwoHours) {
    CronMatcher cm("0 */2 * * *");
    DateTime dt = {2024, 1, 15, 0, 0, 0};
    EXPECT_TRUE(cm.matches(dt));
    DateTime dt2 = {2024, 1, 15, 2, 0, 0};
    EXPECT_TRUE(cm.matches(dt2));
    DateTime dt3 = {2024, 1, 15, 3, 0, 0};
    EXPECT_FALSE(cm.matches(dt3));
}
