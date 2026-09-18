#include <gtest/gtest.h>
#include "src/drivers/watermeter/kamstrup/utils/CRC.hpp"
#include "src/drivers/watermeter/kamstrup/utils/CRC.cpp"

using namespace drivers::watermeter::kamstrup::transport::utils;

TEST(CRC16_CCITT, EmptyDataReturnsZero) {
    EXPECT_EQ(crc16_ccitt(nullptr, 0), 0x0000);
}

TEST(CRC16_CCITT, FullFrame) {
    const uint8_t data[] = {0x3F, 0x10, 0x00, 0x44, 0x28, 0x04, 0x43, 0x00, 0x00, 0x15, 0xC0};
    EXPECT_EQ(crc16_ccitt(data, sizeof(data)), 0x38D5);
}

TEST(CRC16_CCITT, FrameWithOneByteStripped) {
    const uint8_t data[] = {0x3F, 0x10, 0x00, 0x44, 0x28, 0x04, 0x43, 0x00, 0x00, 0xC0};
    EXPECT_EQ(crc16_ccitt(data, sizeof(data)), 0x29BE);
}

TEST(CRC16_CCITT, VectorOverloadEmpty) {
    std::vector<uint8_t> empty;
    EXPECT_EQ(crc16_ccitt(empty), 0);
}

TEST(CRC16_CCITT, VectorOverloadMatchesPointer) {
    std::vector<uint8_t> data = {0x3F, 0x10, 0x02};
    EXPECT_EQ(crc16_ccitt(data), crc16_ccitt(data.data(), data.size()));
}
