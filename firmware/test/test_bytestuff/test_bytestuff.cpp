#include <gtest/gtest.h>
#include "src/drivers/watermeter/kamstrup/utils/ByteStuff.hpp"
#include "src/drivers/watermeter/kamstrup/utils/ByteStuff.cpp"

using namespace drivers::watermeter::kamstrup::transport::utils;

TEST(ByteStuff, StuffEmpty) {
    auto result = stuff_bytes({});
    EXPECT_TRUE(result.empty());
}

TEST(ByteStuff, StuffNoReservedBytes) {
    auto result = stuff_bytes({0x41, 0x42, 0x43});
    std::vector<uint8_t> expected = {0x41, 0x42, 0x43};
    EXPECT_EQ(result, expected);
}

TEST(ByteStuff, StuffACK) {
    auto result = stuff_bytes({0x06});
    std::vector<uint8_t> expected = {0x1B, 0xF9};
    EXPECT_EQ(result, expected);
}

TEST(ByteStuff, StuffSTOP) {
    auto result = stuff_bytes({0x0D});
    std::vector<uint8_t> expected = {0x1B, 0xF2};
    EXPECT_EQ(result, expected);
}

TEST(ByteStuff, StuffESC) {
    auto result = stuff_bytes({0x1B});
    std::vector<uint8_t> expected = {0x1B, 0xE4};
    EXPECT_EQ(result, expected);
}

TEST(ByteStuff, StuffStartToHost) {
    auto result = stuff_bytes({0x40});
    std::vector<uint8_t> expected = {0x1B, 0xBF};
    EXPECT_EQ(result, expected);
}

TEST(ByteStuff, StuffStartToMeter) {
    auto result = stuff_bytes({0x80});
    std::vector<uint8_t> expected = {0x1B, 0x7F};
    EXPECT_EQ(result, expected);
}

TEST(ByteStuff, StuffAllReservedAtOnce) {
    auto result = stuff_bytes({0x06, 0x0D, 0x1B, 0x40, 0x80});
    std::vector<uint8_t> expected = {0x1B, 0xF9, 0x1B, 0xF2, 0x1B, 0xE4, 0x1B, 0xBF, 0x1B, 0x7F};
    EXPECT_EQ(result, expected);
}

TEST(ByteStuff, StuffNAK_NotReserved) {
    auto result = stuff_bytes({0x15});
    std::vector<uint8_t> expected = {0x15};
    EXPECT_EQ(result, expected);
}

TEST(ByteStuff, UnstuffEmpty) {
    auto result = unstuff_bytes({});
    EXPECT_TRUE(result.empty());
}

TEST(ByteStuff, UnstuffNoEscapes) {
    auto result = unstuff_bytes({0x41, 0x42, 0x43});
    std::vector<uint8_t> expected = {0x41, 0x42, 0x43};
    EXPECT_EQ(result, expected);
}

TEST(ByteStuff, UnstuffACK) {
    auto result = unstuff_bytes({0x1B, 0xF9});
    std::vector<uint8_t> expected = {0x06};
    EXPECT_EQ(result, expected);
}

TEST(ByteStuff, UnstuffSTOP) {
    auto result = unstuff_bytes({0x1B, 0xF2});
    std::vector<uint8_t> expected = {0x0D};
    EXPECT_EQ(result, expected);
}

TEST(ByteStuff, UnstuffTrailingESC_DropsSilently) {
    auto result = unstuff_bytes({0x1B});
    EXPECT_TRUE(result.empty());
}

TEST(ByteStuff, UnstuffESC_0x00_Equals_0xFF) {
    auto result = unstuff_bytes({0x1B, 0x00});
    std::vector<uint8_t> expected = {0xFF};
    EXPECT_EQ(result, expected);
}

TEST(ByteStuff, RoundTripNoReservedBytes) {
    std::vector<uint8_t> original = {0x3F, 0x10, 0x00, 0x44, 0x28, 0x04, 0x43, 0x00, 0x00, 0x15, 0xC0};
    EXPECT_EQ(unstuff_bytes(stuff_bytes(original)), original);
}
