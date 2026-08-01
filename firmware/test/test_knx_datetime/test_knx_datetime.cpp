#include <gtest/gtest.h>
#include "src/utils/knx/KnxDateTimeParser.hpp"
#include "src/utils/knx/KnxDateTimeParser.cpp"

using namespace utils::knx;

class KnxTimeParserTest : public ::testing::Test {};
class KnxDateParserTest : public ::testing::Test {};

TEST_F(KnxTimeParserTest, ValidTime) {
    const uint8_t data[] = {0x0A, 0x1E, 0x2D};
    KnxTime t = parseKnxTime(data, sizeof(data));
    EXPECT_TRUE(t.valid);
    EXPECT_EQ(10, t.hour);
    EXPECT_EQ(30, t.minute);
    EXPECT_EQ(45, t.second);
}

TEST_F(KnxTimeParserTest, WeekdayBitsIgnored) {
    const uint8_t data[] = {0x2A, 0x00, 0x00};
    KnxTime t = parseKnxTime(data, sizeof(data));
    EXPECT_TRUE(t.valid);
    EXPECT_EQ(10, t.hour);
}

TEST_F(KnxTimeParserTest, MaxBoundaryValid) {
    const uint8_t data[] = {0x17, 0x3B, 0x3B};
    KnxTime t = parseKnxTime(data, sizeof(data));
    EXPECT_TRUE(t.valid);
    EXPECT_EQ(23, t.hour);
    EXPECT_EQ(59, t.minute);
    EXPECT_EQ(59, t.second);
}

TEST_F(KnxTimeParserTest, InvalidHour) {
    const uint8_t data[] = {0x18, 0x00, 0x00};
    KnxTime t = parseKnxTime(data, sizeof(data));
    EXPECT_FALSE(t.valid);
}

TEST_F(KnxTimeParserTest, InvalidMinute) {
    const uint8_t data[] = {0x0A, 0x3C, 0x00};
    KnxTime t = parseKnxTime(data, sizeof(data));
    EXPECT_FALSE(t.valid);
}

TEST_F(KnxTimeParserTest, InvalidSecond) {
    const uint8_t data[] = {0x0A, 0x00, 0x3C};
    KnxTime t = parseKnxTime(data, sizeof(data));
    EXPECT_FALSE(t.valid);
}

TEST_F(KnxTimeParserTest, NullDataWithValidSize) {
    KnxTime t = parseKnxTime(nullptr, 3);
    EXPECT_FALSE(t.valid);
}

TEST_F(KnxTimeParserTest, EmptyPayload) {
    const uint8_t data[] = {0x0A};
    KnxTime t = parseKnxTime(data, 0);
    EXPECT_FALSE(t.valid);
}

TEST_F(KnxTimeParserTest, RejectsDpt19SizedTelegram) {
    const uint8_t data[] = {0x0A, 0x1E, 0x2D, 0x00, 0x00, 0x00, 0x00, 0x00};
    KnxTime t = parseKnxTime(data, sizeof(data));
    EXPECT_FALSE(t.valid);
}

TEST_F(KnxTimeParserTest, MinuteSecondHighBitsMasked) {
    const uint8_t data[] = {0x0A, 0x5E, 0x6D};
    KnxTime t = parseKnxTime(data, sizeof(data));
    EXPECT_TRUE(t.valid);
    EXPECT_EQ(10, t.hour);
    EXPECT_EQ(30, t.minute);
    EXPECT_EQ(45, t.second);
}

TEST_F(KnxTimeParserTest, MidnightTime) {
    const uint8_t data[] = {0x00, 0x00, 0x00};
    KnxTime t = parseKnxTime(data, sizeof(data));
    EXPECT_TRUE(t.valid);
    EXPECT_EQ(0, t.hour);
    EXPECT_EQ(0, t.minute);
    EXPECT_EQ(0, t.second);
}

TEST_F(KnxTimeParserTest, InvalidInputZeroesTimeFields) {
    const uint8_t data[] = {0x0A, 0x3C, 0x00};
    KnxTime t = parseKnxTime(data, sizeof(data));
    EXPECT_FALSE(t.valid);
    EXPECT_EQ(0, t.hour);
    EXPECT_EQ(0, t.minute);
    EXPECT_EQ(0, t.second);
}

TEST_F(KnxDateParserTest, ValidDate) {
    const uint8_t data[] = {0x0F, 0x06, 0x18};
    KnxDate d = parseKnxDate(data, sizeof(data));
    EXPECT_TRUE(d.valid);
    EXPECT_EQ(2024, d.year);
    EXPECT_EQ(6, d.month);
    EXPECT_EQ(15, d.day);
}

TEST_F(KnxDateParserTest, DayHighNibbleBitsMasked) {
    const uint8_t data[] = {0x5F, 0x07, 0x18};
    KnxDate d = parseKnxDate(data, sizeof(data));
    EXPECT_TRUE(d.valid);
    EXPECT_EQ(31, d.day);
}

TEST_F(KnxDateParserTest, MonthHighNibbleBitsMasked) {
    const uint8_t data[] = {0x0F, 0x56, 0x18};
    KnxDate d = parseKnxDate(data, sizeof(data));
    EXPECT_TRUE(d.valid);
    EXPECT_EQ(6, d.month);
}

TEST_F(KnxDateParserTest, YearField89MapsTo2089) {
    const uint8_t data[] = {0x0F, 0x06, 0x59};
    KnxDate d = parseKnxDate(data, sizeof(data));
    EXPECT_TRUE(d.valid);
    EXPECT_EQ(2089, d.year);
}

TEST_F(KnxDateParserTest, YearField90MapsTo1990) {
    const uint8_t data[] = {0x0F, 0x06, 0x5A};
    KnxDate d = parseKnxDate(data, sizeof(data));
    EXPECT_TRUE(d.valid);
    EXPECT_EQ(1990, d.year);
}

TEST_F(KnxDateParserTest, InvalidMonthZero) {
    const uint8_t data[] = {0x0F, 0x00, 0x18};
    KnxDate d = parseKnxDate(data, sizeof(data));
    EXPECT_FALSE(d.valid);
}

TEST_F(KnxDateParserTest, InvalidMonthThirteen) {
    const uint8_t data[] = {0x0F, 0x0D, 0x18};
    KnxDate d = parseKnxDate(data, sizeof(data));
    EXPECT_FALSE(d.valid);
}

TEST_F(KnxDateParserTest, InvalidDayZero) {
    const uint8_t data[] = {0x00, 0x06, 0x18};
    KnxDate d = parseKnxDate(data, sizeof(data));
    EXPECT_FALSE(d.valid);
}

TEST_F(KnxDateParserTest, InvalidYearField100) {
    const uint8_t data[] = {0x0F, 0x06, 0x64};
    KnxDate d = parseKnxDate(data, sizeof(data));
    EXPECT_FALSE(d.valid);
}

TEST_F(KnxDateParserTest, NullDataWithValidSize) {
    KnxDate d = parseKnxDate(nullptr, 3);
    EXPECT_FALSE(d.valid);
}

TEST_F(KnxDateParserTest, WrongSize) {
    const uint8_t data[] = {0x0F, 0x06};
    KnxDate d = parseKnxDate(data, sizeof(data));
    EXPECT_FALSE(d.valid);
}

TEST_F(KnxDateParserTest, RejectsDpt19SizedTelegram) {
    const uint8_t data[] = {0x0F, 0x06, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00};
    KnxDate d = parseKnxDate(data, sizeof(data));
    EXPECT_FALSE(d.valid);
}

TEST_F(KnxDateParserTest, YearByteHighBitMasked) {
    const uint8_t data[] = {0x0F, 0x06, 0x98};
    KnxDate d = parseKnxDate(data, sizeof(data));
    EXPECT_TRUE(d.valid);
    EXPECT_EQ(2024, d.year);
}

TEST_F(KnxDateParserTest, YearField0MapsTo2000) {
    const uint8_t data[] = {0x0F, 0x06, 0x00};
    KnxDate d = parseKnxDate(data, sizeof(data));
    EXPECT_TRUE(d.valid);
    EXPECT_EQ(2000, d.year);
}

TEST_F(KnxDateParserTest, YearField99MapsTo1999) {
    const uint8_t data[] = {0x0F, 0x06, 0x63};
    KnxDate d = parseKnxDate(data, sizeof(data));
    EXPECT_TRUE(d.valid);
    EXPECT_EQ(1999, d.year);
}

TEST_F(KnxDateParserTest, February29LeapYearValid) {
    const uint8_t data[] = {0x1D, 0x02, 0x18};
    KnxDate d = parseKnxDate(data, sizeof(data));
    EXPECT_TRUE(d.valid);
    EXPECT_EQ(2024, d.year);
    EXPECT_EQ(2, d.month);
    EXPECT_EQ(29, d.day);
}

TEST_F(KnxDateParserTest, February29NonLeapYearInvalid) {
    const uint8_t data[] = {0x1D, 0x02, 0x17};
    KnxDate d = parseKnxDate(data, sizeof(data));
    EXPECT_FALSE(d.valid);
}

TEST_F(KnxDateParserTest, February31Invalid) {
    const uint8_t data[] = {0x1F, 0x02, 0x18};
    KnxDate d = parseKnxDate(data, sizeof(data));
    EXPECT_FALSE(d.valid);
}

TEST_F(KnxDateParserTest, April31Invalid) {
    const uint8_t data[] = {0x1F, 0x04, 0x18};
    KnxDate d = parseKnxDate(data, sizeof(data));
    EXPECT_FALSE(d.valid);
}

TEST_F(KnxDateParserTest, April30Valid) {
    const uint8_t data[] = {0x1E, 0x04, 0x18};
    KnxDate d = parseKnxDate(data, sizeof(data));
    EXPECT_TRUE(d.valid);
    EXPECT_EQ(2024, d.year);
    EXPECT_EQ(4, d.month);
    EXPECT_EQ(30, d.day);
}

TEST_F(KnxDateParserTest, December31Valid) {
    const uint8_t data[] = {0x1F, 0x0C, 0x18};
    KnxDate d = parseKnxDate(data, sizeof(data));
    EXPECT_TRUE(d.valid);
    EXPECT_EQ(2024, d.year);
    EXPECT_EQ(12, d.month);
    EXPECT_EQ(31, d.day);
}

TEST_F(KnxDateParserTest, InvalidInputZeroesDateFields) {
    const uint8_t data[] = {0x0F, 0x0D, 0x18};
    KnxDate d = parseKnxDate(data, sizeof(data));
    EXPECT_FALSE(d.valid);
    EXPECT_EQ(0, d.year);
    EXPECT_EQ(0, d.month);
    EXPECT_EQ(0, d.day);
}
