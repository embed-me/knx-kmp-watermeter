#include <gtest/gtest.h>
#include "src/drivers/watermeter/kamstrup/transport/layers/application/ApplicationLayer.hpp"
#include "../mocks/mock_data_link_layer.hpp"

#include "src/drivers/watermeter/kamstrup/transport/layers/application/ApplicationLayer.cpp"

using namespace drivers::watermeter::kamstrup::transport;

class ApplicationLayerTest : public ::testing::Test {
protected:
    void SetUp() override {
        mockDl = std::make_shared<MockDataLinkLayer>();
        app = std::make_shared<ApplicationLayer>(mockDl);
    }

    std::shared_ptr<MockDataLinkLayer> mockDl;
    std::shared_ptr<ApplicationLayer> app;
};

TEST_F(ApplicationLayerTest, SendRequestPrependsCid) {
    app->sendRequest(0x10, {0x00, 0x44});

    std::vector<uint8_t> expected = {0x10, 0x00, 0x44};
    EXPECT_EQ(mockDl->lastPayload, expected);
}

TEST_F(ApplicationLayerTest, RegisterHandlerForCid_ReceivesPayload) {
    std::vector<uint8_t> received;
    app->registerHandler(0x10, [&](const std::vector<uint8_t>& data) { received = data; });

    mockDl->simulateReceive({0x10, 0x00, 0x44, 0x28});

    std::vector<uint8_t> expected = {0x00, 0x44, 0x28};
    EXPECT_EQ(received, expected);
}

TEST_F(ApplicationLayerTest, UnknownCid_NoHandlerCalled) {
    bool called = false;
    app->registerHandler(0x10, [&](const std::vector<uint8_t>&) { called = true; });

    mockDl->simulateReceive({0x20, 0x00}); // CID 0x20, no handler

    EXPECT_FALSE(called);
}

TEST_F(ApplicationLayerTest, EmptyFrame_WarningNoHandlerCalled) {
    bool called = false;
    app->registerHandler(0x00, [&](const std::vector<uint8_t>&) { called = true; });

    mockDl->simulateReceive({}); // empty frame

    EXPECT_FALSE(called);
}

TEST_F(ApplicationLayerTest, AckReceived_NotifiesListener) {
    AckType received = AckType::Nak;
    app->registerAckListener([&](AckType type) { received = type; });

    mockDl->simulateAck(AckType::Ack);

    EXPECT_EQ(received, AckType::Ack);
}

TEST_F(ApplicationLayerTest, MultipleHandlersForSameCid_AllCalled) {
    int count = 0;
    app->registerHandler(0x10, [&](const std::vector<uint8_t>&) { count++; });
    app->registerHandler(0x10, [&](const std::vector<uint8_t>&) { count++; });

    mockDl->simulateReceive({0x10, 0x00});

    EXPECT_EQ(count, 2);
}

TEST_F(ApplicationLayerTest, SendRequestWithNullDataLink_NoCrash) {
    auto appNoDl = std::make_shared<ApplicationLayer>(nullptr);
    EXPECT_NO_THROW(appNoDl->sendRequest(0x10, {0x00}));
}
