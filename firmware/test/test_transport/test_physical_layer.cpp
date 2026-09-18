#include <gtest/gtest.h>
#include "src/drivers/watermeter/kamstrup/transport/layers/physical/PhysicalLayer.hpp"
#include "../mocks/mock_uart_driver.hpp"

#include "src/drivers/logger/Logger.cpp"
#include "src/drivers/watermeter/kamstrup/utils/ByteStuff.cpp"
#include "src/drivers/watermeter/kamstrup/transport/layers/physical/PhysicalLayer.cpp"

using namespace drivers::watermeter::kamstrup::transport;

class PhysicalLayerTest : public ::testing::Test {
protected:
    void SetUp() override {
        mockUart = std::make_shared<MockUartDriver>();
        phy = std::make_shared<PhysicalLayer>(mockUart);
    }

    std::shared_ptr<MockUartDriver> mockUart;
    std::shared_ptr<PhysicalLayer> phy;
};

TEST_F(PhysicalLayerTest, SendFrameWrapsWithStartStop) {
    phy->sendFrame({0x3F, 0x10, 0x02});

    ASSERT_GE(mockUart->lastWritten.size(), 3u);
    EXPECT_EQ(mockUart->lastWritten[0], 0x80); // START_TO_METER
    EXPECT_EQ(mockUart->lastWritten.back(), 0x0D); // STOP
}

TEST_F(PhysicalLayerTest, SendFrameStuffsReservedBytes) {
    phy->sendFrame({0x06});

    std::vector<uint8_t> expected = {0x80, 0x1B, 0xF9, 0x0D};
    EXPECT_EQ(mockUart->lastWritten, expected);
}

TEST_F(PhysicalLayerTest, ReceiveFrameDeliversUnstuffedPayload) {
    std::vector<uint8_t> received;
    phy->registerListener([&](const std::vector<uint8_t>& data) { received = data; });

    mockUart->receiveBytes({0x40, 0x3F, 0x10, 0x02, 0x38, 0xD5, 0x0D});

    std::vector<uint8_t> expected = {0x3F, 0x10, 0x02, 0x38, 0xD5};
    EXPECT_EQ(received, expected);
}

TEST_F(PhysicalLayerTest, AckOutsideFrame) {
    AckType receivedAck = AckType::Nak;
    phy->registerAckListener([&](AckType type) { receivedAck = type; });

    mockUart->receiveBytes({0x06});

    EXPECT_EQ(receivedAck, AckType::Ack);
}

TEST_F(PhysicalLayerTest, NakOutsideFrame) {
    AckType receivedAck = AckType::Ack;
    phy->registerAckListener([&](AckType type) { receivedAck = type; });

    mockUart->receiveBytes({0x15});

    EXPECT_EQ(receivedAck, AckType::Nak);
}

TEST_F(PhysicalLayerTest, NakInsideFrame_NotStripped) {
    std::vector<uint8_t> received;
    phy->registerListener([&](const std::vector<uint8_t>& data) { received = data; });

    mockUart->receiveBytes({0x40, 0x15, 0x0D});

    std::vector<uint8_t> expected = {0x15};
    EXPECT_EQ(received, expected);
}

TEST_F(PhysicalLayerTest, AckInsideFrame_NotStripped) {
    std::vector<uint8_t> received;
    phy->registerListener([&](const std::vector<uint8_t>& data) { received = data; });

    mockUart->receiveBytes({0x40, 0x06, 0x0D});

    std::vector<uint8_t> expected = {0x06};
    EXPECT_EQ(received, expected);
}

TEST_F(PhysicalLayerTest, FrameWithStuffedBytes_UnstuffsCorrectly) {
    std::vector<uint8_t> received;
    phy->registerListener([&](const std::vector<uint8_t>& data) { received = data; });

    mockUart->receiveBytes({0x40, 0x1B, 0xF9, 0x0D});

    std::vector<uint8_t> expected = {0x06};
    EXPECT_EQ(received, expected);
}

TEST_F(PhysicalLayerTest, NoiseThenValidFrame) {
    std::vector<uint8_t> received;
    phy->registerListener([&](const std::vector<uint8_t>& data) { received = data; });

    mockUart->receiveBytes({0xFF, 0x40, 0x3F, 0x10, 0x0D});

    std::vector<uint8_t> expected = {0x3F, 0x10};
    EXPECT_EQ(received, expected);
}

TEST_F(PhysicalLayerTest, MalformedStop_NoAckSwallowedAfter) {
    AckType receivedAck = AckType::Nak;
    phy->registerAckListener([&](AckType type) { receivedAck = type; });

    mockUart->receiveBytes({0x0D}); // noise STOP with no frame
    mockUart->receiveBytes({0x06}); // ACK should still work

    EXPECT_EQ(receivedAck, AckType::Ack);
}
