#include <gtest/gtest.h>
#include "src/drivers/watermeter/kamstrup/utils/KmpUnits.hpp"

using namespace drivers::watermeter::kamstrup::transport;

TEST(KmpUnits, UnspecifiedReturnsEmptyString) {
    EXPECT_EQ(unitToString(KmpUnit::UNSPECIFIED), "");
}

TEST(KmpUnits, Celsius) {
    EXPECT_EQ(unitToString(KmpUnit::C), "C");
}

TEST(KmpUnits, Liters) {
    EXPECT_EQ(unitToString(KmpUnit::L), "l");
}

TEST(KmpUnits, LitersPerHour) {
    EXPECT_EQ(unitToString(KmpUnit::L_PER_H), "l/h");
}

TEST(KmpUnits, Hours) {
    EXPECT_EQ(unitToString(KmpUnit::H), "h");
}

TEST(KmpUnits, Clock) {
    EXPECT_EQ(unitToString(KmpUnit::CLOCK), "clock");
}

TEST(KmpUnits, M3_CurrentBug) {
    EXPECT_EQ(unitToString(KmpUnit::M3), "m3");
}

TEST(KmpUnits, UnknownEnumValue) {
    EXPECT_EQ(unitToString(KmpUnit::UNKNOWN), "UNKNOWN");
}

TEST(KmpUnits, UnmappedByte0x10) {
    EXPECT_EQ(unitToString(static_cast<KmpUnit>(0x10)), "UNKNOWN");
}