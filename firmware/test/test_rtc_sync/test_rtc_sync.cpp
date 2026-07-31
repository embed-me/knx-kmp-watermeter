#include <gtest/gtest.h>
#include "src/utils/knx/KnxDateTimeParser.hpp"
#include "src/utils/knx/KnxDateTimeParser.cpp"
#include "src/utils/knx/RtcSyncState.hpp"
#include "src/utils/knx/RtcSyncState.cpp"

using namespace utils::knx;
using namespace drivers::rtc;

class RtcSyncStateTest : public ::testing::Test {};

// Wire bytes match test_knx_datetime: time {0x0A,0x1E,0x2D} -> 10:30:45,
// date {0x0F,0x06,0x18} -> 2024-06-15, {0x0F,0x06,0x00} -> 2000-06-15.
static const uint8_t kTime1045[] = {0x0A, 0x1E, 0x2D};    // 10:30:45
static const uint8_t kTime1145[] = {0x0B, 0x1E, 0x2D};    // 11:30:45
static const uint8_t kTimeMidnight[] = {0x00, 0x00, 0x00}; // 00:00:00
static const uint8_t kDate2024_06_15[] = {0x0F, 0x06, 0x18};
static const uint8_t kDate2024_07_30[] = {0x1E, 0x07, 0x18};
static const uint8_t kDate2000_06_15[] = {0x0F, 0x06, 0x00};

TEST_F(RtcSyncStateTest, DefaultInitializationZeroesAllFields) {
    RtcSyncState state;
    const DateTime& dt = state.dateTime();
    EXPECT_EQ(0, dt.year);
    EXPECT_EQ(0, dt.month);
    EXPECT_EQ(0, dt.day);
    EXPECT_EQ(0, dt.hour);
    EXPECT_EQ(0, dt.minute);
    EXPECT_EQ(0, dt.second);
}

TEST_F(RtcSyncStateTest, TimeOnlyStoresTimeButDoesNotReady) {
    RtcSyncState state;
    KnxTime t = parseKnxTime(kTime1045, sizeof(kTime1045));
    EXPECT_FALSE(state.updateTime(t));

    const DateTime& dt = state.dateTime();
    EXPECT_EQ(10, dt.hour);
    EXPECT_EQ(30, dt.minute);
    EXPECT_EQ(45, dt.second);
    // No write would occur: date component still zero.
    EXPECT_EQ(0, dt.year);
    EXPECT_EQ(0, dt.month);
    EXPECT_EQ(0, dt.day);
}

TEST_F(RtcSyncStateTest, DateOnlyStoresDateButDoesNotReady) {
    RtcSyncState state;
    KnxDate d = parseKnxDate(kDate2024_06_15, sizeof(kDate2024_06_15));
    EXPECT_FALSE(state.updateDate(d));

    const DateTime& dt = state.dateTime();
    EXPECT_EQ(2024, dt.year);
    EXPECT_EQ(6, dt.month);
    EXPECT_EQ(15, dt.day);
    // No write would occur: time component still zero.
    EXPECT_EQ(0, dt.hour);
    EXPECT_EQ(0, dt.minute);
    EXPECT_EQ(0, dt.second);
}

TEST_F(RtcSyncStateTest, TimeThenDateReadiesAndMergesAllFields) {
    RtcSyncState state;
    KnxTime t = parseKnxTime(kTime1045, sizeof(kTime1045));
    KnxDate d = parseKnxDate(kDate2024_06_15, sizeof(kDate2024_06_15));

    EXPECT_FALSE(state.updateTime(t));
    EXPECT_TRUE(state.updateDate(d));

    const DateTime& dt = state.dateTime();
    EXPECT_EQ(2024, dt.year);
    EXPECT_EQ(6, dt.month);
    EXPECT_EQ(15, dt.day);
    EXPECT_EQ(10, dt.hour);
    EXPECT_EQ(30, dt.minute);
    EXPECT_EQ(45, dt.second);
}

TEST_F(RtcSyncStateTest, DateThenTimeReadiesAndMergesAllFields) {
    RtcSyncState state;
    KnxDate d = parseKnxDate(kDate2024_06_15, sizeof(kDate2024_06_15));
    KnxTime t = parseKnxTime(kTime1045, sizeof(kTime1045));

    EXPECT_FALSE(state.updateDate(d));
    EXPECT_TRUE(state.updateTime(t));

    const DateTime& dt = state.dateTime();
    EXPECT_EQ(2024, dt.year);
    EXPECT_EQ(6, dt.month);
    EXPECT_EQ(15, dt.day);
    EXPECT_EQ(10, dt.hour);
    EXPECT_EQ(30, dt.minute);
    EXPECT_EQ(45, dt.second);
}

TEST_F(RtcSyncStateTest, RepeatedTimeAfterReadyKeepsLastKnownDate) {
    RtcSyncState state;
    KnxTime t1 = parseKnxTime(kTime1045, sizeof(kTime1045));
    KnxTime t2 = parseKnxTime(kTime1145, sizeof(kTime1145));
    KnxDate d = parseKnxDate(kDate2024_06_15, sizeof(kDate2024_06_15));

    state.updateTime(t1);
    EXPECT_TRUE(state.updateDate(d));
    EXPECT_TRUE(state.updateTime(t2));

    const DateTime& dt = state.dateTime();
    EXPECT_EQ(11, dt.hour);
    EXPECT_EQ(30, dt.minute);
    EXPECT_EQ(45, dt.second);
    EXPECT_EQ(2024, dt.year);
    EXPECT_EQ(6, dt.month);
    EXPECT_EQ(15, dt.day);
}

TEST_F(RtcSyncStateTest, RepeatedDateAfterReadyKeepsLastKnownTime) {
    RtcSyncState state;
    KnxDate d1 = parseKnxDate(kDate2024_06_15, sizeof(kDate2024_06_15));
    KnxDate d2 = parseKnxDate(kDate2024_07_30, sizeof(kDate2024_07_30));
    KnxTime t = parseKnxTime(kTime1045, sizeof(kTime1045));

    state.updateDate(d1);
    EXPECT_TRUE(state.updateTime(t));
    EXPECT_TRUE(state.updateDate(d2));

    const DateTime& dt = state.dateTime();
    EXPECT_EQ(2024, dt.year);
    EXPECT_EQ(7, dt.month);
    EXPECT_EQ(30, dt.day);
    EXPECT_EQ(10, dt.hour);
    EXPECT_EQ(30, dt.minute);
    EXPECT_EQ(45, dt.second);
}

TEST_F(RtcSyncStateTest, MidnightTimeIsNotMistakenForMissing) {
    RtcSyncState state;
    KnxDate d = parseKnxDate(kDate2024_06_15, sizeof(kDate2024_06_15));
    KnxTime midnight = parseKnxTime(kTimeMidnight, sizeof(kTimeMidnight));
    EXPECT_TRUE(midnight.valid);
    EXPECT_TRUE(midnight.hour == 0 && midnight.minute == 0 && midnight.second == 0);

    state.updateDate(d);
    EXPECT_TRUE(state.updateTime(midnight));

    const DateTime& dt = state.dateTime();
    EXPECT_EQ(0, dt.hour);
    EXPECT_EQ(0, dt.minute);
    EXPECT_EQ(0, dt.second);
    EXPECT_EQ(2024, dt.year);
    EXPECT_EQ(6, dt.month);
    EXPECT_EQ(15, dt.day);
}

TEST_F(RtcSyncStateTest, YearEndpointDateMapsYearFieldZeroTo2000) {
    RtcSyncState state;
    KnxDate d = parseKnxDate(kDate2000_06_15, sizeof(kDate2000_06_15));
    KnxTime t = parseKnxTime(kTime1045, sizeof(kTime1045));
    EXPECT_EQ(2000, d.year);

    state.updateDate(d);
    EXPECT_TRUE(state.updateTime(t));

    const DateTime& dt = state.dateTime();
    EXPECT_EQ(2000, dt.year);
    EXPECT_EQ(6, dt.month);
    EXPECT_EQ(15, dt.day);
    EXPECT_EQ(10, dt.hour);
    EXPECT_EQ(30, dt.minute);
    EXPECT_EQ(45, dt.second);
}
