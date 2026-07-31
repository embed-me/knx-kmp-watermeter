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
    const uint8_t data[] = {};
    KnxTime t = parseKnxTime(data, 0);
    EXPECT_FALSE(t.valid);
}

TEST_F(KnxTimeParserTest, RejectsDpt19SizedTelegram) {
    const uint8_t data[] = {0x0A, 0x1E, 0x2D, 0x00, 0x00, 0x00, 0x00, 0x00};
    KnxTime t = parseKnxTime(data, sizeof(data));
    EXPECT_FALSE(t.valid);
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
    const uint8_t data[] = {0x5F, 0x06, 0x18};
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
