#include <gtest/gtest.h>
#include "src/drivers/watermeter/kamstrup/transport/layers/data_link/DataLinkLayer.hpp"
#include "src/drivers/watermeter/kamstrup/utils/CRC.hpp"
#include "../mocks/mock_physical_layer.hpp"

#include "src/drivers/watermeter/kamstrup/utils/CRC.cpp"
#include "src/drivers/watermeter/kamstrup/transport/layers/data_link/DataLinkLayer.cpp"

using namespace drivers::watermeter::kamstrup::transport;

class DataLinkLayerTest : public ::testing::Test {
protected:
    void SetUp() override {
        mockPhy = std::make_shared<MockPhysicalLayer>();
        dll = std::make_shared<DataLinkLayer>(mockPhy, 0x3F);
    }

    std::shared_ptr<MockPhysicalLayer> mockPhy;
    std::shared_ptr<DataLinkLayer> dll;
};

TEST_F(DataLinkLayerTest, SendApplicationPayload_AddsDestAndCRC) {
    dll->sendApplicationPayload({0x10, 0x02});

    ASSERT_GE(mockPhy->lastFrame.size(), 5u);
    EXPECT_EQ(mockPhy->lastFrame[0], 0x3F); // destination

    std::vector<uint8_t> dataForCrc(mockPhy->lastFrame.begin(),
                                    mockPhy->lastFrame.end() - 2);
    uint16_t calc = drivers::watermeter::kamstrup::transport::utils::crc16_ccitt(dataForCrc);
    uint16_t stored = (static_cast<uint16_t>(mockPhy->lastFrame[mockPhy->lastFrame.size() - 2]) << 8)
                    | mockPhy->lastFrame[mockPhy->lastFrame.size() - 1];
    EXPECT_EQ(calc, stored);
}

TEST_F(DataLinkLayerTest, ReceiveValidFrame_DeliversAppPayload) {
    std::vector<uint8_t> received;
    dll->registerListener([&](const std::vector<uint8_t>& data) { received = data; });

    std::vector<uint8_t> frame = {0x3F, 0x10, 0x02};
    uint16_t crc = drivers::watermeter::kamstrup::transport::utils::crc16_ccitt(frame);
    frame.push_back(static_cast<uint8_t>(crc >> 8));
    frame.push_back(static_cast<uint8_t>(crc & 0xFF));

    mockPhy->simulateFrame(frame);

    std::vector<uint8_t> expected = {0x10, 0x02};
    EXPECT_EQ(received, expected);
}

TEST_F(DataLinkLayerTest, ReceiveFrameWithCrcMismatch_NotDelivered) {
    bool called = false;
    dll->registerListener([&](const std::vector<uint8_t>&) { called = true; });

    std::vector<uint8_t> frame = {0x3F, 0x10, 0x02, 0x00, 0x00}; // wrong CRC
    mockPhy->simulateFrame(frame);

    EXPECT_FALSE(called);
}

TEST_F(DataLinkLayerTest, ReceiveFrameWrongDestination_NotDelivered) {
    bool called = false;
    dll->registerListener([&](const std::vector<uint8_t>&) { called = true; });

    std::vector<uint8_t> frame = {0x00, 0x10, 0x02}; // dest=0x00, ours=0x3F
    uint16_t crc = drivers::watermeter::kamstrup::transport::utils::crc16_ccitt(frame);
    frame.push_back(static_cast<uint8_t>(crc >> 8));
    frame.push_back(static_cast<uint8_t>(crc & 0xFF));

    mockPhy->simulateFrame(frame);

    EXPECT_FALSE(called);
}

TEST_F(DataLinkLayerTest, ReceiveFrameTooSmall_NotDelivered) {
    bool called = false;
    dll->registerListener([&](const std::vector<uint8_t>&) { called = true; });

    mockPhy->simulateFrame({0x3F, 0x10});

    EXPECT_FALSE(called);
}

TEST_F(DataLinkLayerTest, AckReceived_NotifiesListener) {
    AckType received = AckType::Nak;
    dll->registerAckListener([&](AckType type) { received = type; });

    mockPhy->simulateAck(AckType::Ack);

    EXPECT_EQ(received, AckType::Ack);
}
