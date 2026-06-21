#include <gtest/gtest.h>
#include "src/drivers/watermeter/kamstrup/commands/GetRegisterCommand.hpp"
#include "src/drivers/watermeter/kamstrup/utils/CommandQueue.hpp"
#include "../mocks/mock_application_layer.hpp"

#include "src/drivers/logger/Logger.cpp"
#include "src/drivers/watermeter/kamstrup/commands/GetRegisterCommand.cpp"

using namespace drivers::watermeter::kamstrup::transport;

class GetRegisterCommandTest : public ::testing::Test {
protected:
    void SetUp() override {
        mockApp = std::make_shared<MockApplicationLayer>();
    }

    std::shared_ptr<MockApplicationLayer> mockApp;
};

TEST_F(GetRegisterCommandTest, ExecuteSendsCorrectCidAndPayload) {
    auto cmd = std::make_shared<GetRegisterCommand>(mockApp, 0x0044);
    cmd->execute();

    EXPECT_TRUE(mockApp->sendRequestCalled);
    EXPECT_EQ(mockApp->lastCid, GetRegisterCommand::CID);
    std::vector<uint8_t> expected = {0x01, 0x00, 0x44};
    EXPECT_EQ(mockApp->lastPayload, expected);
}

TEST_F(GetRegisterCommandTest, ExecuteWithNullApp_NoCrash) {
    auto cmd = std::make_shared<GetRegisterCommand>(nullptr, 0x0044);
    EXPECT_NO_THROW(cmd->execute());
}

TEST_F(GetRegisterCommandTest, OnResultTimeout) {
    auto cmd = std::make_shared<GetRegisterCommand>(mockApp, 0x0044);
    CommandResult received;
    cmd->registerListener([&](const CommandResult& r) { received = r; });
    cmd->onExecuteResult(ExecuteResult::TIMEOUT, {});

    EXPECT_EQ(received.result, CommandResult::Result::TIMEOUT);
    EXPECT_EQ(received.register_id, 0x0044);
}

TEST_F(GetRegisterCommandTest, OnResultCorruptTooShort) {
    auto cmd = std::make_shared<GetRegisterCommand>(mockApp, 0x0044);
    CommandResult received;
    cmd->registerListener([&](const CommandResult& r) { received = r; });
    cmd->onExecuteResult(ExecuteResult::SUCCESS, {0xAA, 0xBB});

    EXPECT_EQ(received.result, CommandResult::Result::CORRUPT);
    EXPECT_EQ(received.register_id, 0x0044);
}

TEST_F(GetRegisterCommandTest, OnResultSuccessEmptyPayload) {
    auto cmd = std::make_shared<GetRegisterCommand>(mockApp, 0x0044);
    CommandResult received;
    cmd->registerListener([&](const CommandResult& r) { received = r; });
    cmd->onExecuteResult(ExecuteResult::SUCCESS, {});

    EXPECT_EQ(received.result, CommandResult::Result::CORRUPT);
}

TEST_F(GetRegisterCommandTest, DecodeVolume_CorrectPath) {
    auto cmd = std::make_shared<GetRegisterCommand>(mockApp, 0x0044);
    CommandResult received;
    cmd->registerListener([&](const CommandResult& r) { received = r; });
    std::vector<uint8_t> payload = {0x00, 0x44, 0x3E, 0x04, 0x43, 0x00, 0x00, 0x15, 0xC0};
    cmd->onExecuteResult(ExecuteResult::SUCCESS, payload);

    EXPECT_EQ(received.result, CommandResult::Result::OK);
    EXPECT_EQ(received.register_id, 0x0044);
    EXPECT_NEAR(received.value, 5.568, 0.001);
    EXPECT_EQ(received.unit, KmpUnit::M3);
}

TEST_F(GetRegisterCommandTest, DecodeFlow) {
    auto cmd = std::make_shared<GetRegisterCommand>(mockApp, 0x004A);
    CommandResult received;
    cmd->registerListener([&](const CommandResult& r) { received = r; });
    std::vector<uint8_t> payload = {0x00, 0x4A, 0x29, 0x04, 0x41, 0x00, 0x00, 0x02, 0xEE};
    cmd->onExecuteResult(ExecuteResult::SUCCESS, payload);

    EXPECT_EQ(received.result, CommandResult::Result::OK);
    EXPECT_EQ(received.register_id, 0x004A);
    EXPECT_NEAR(received.value, 75.0, 0.01);
    EXPECT_EQ(received.unit, KmpUnit::L_PER_H);
}

TEST_F(GetRegisterCommandTest, DecodeTemperature) {
    auto cmd = std::make_shared<GetRegisterCommand>(mockApp, 0x0124);
    CommandResult received;
    cmd->registerListener([&](const CommandResult& r) { received = r; });
    std::vector<uint8_t> payload = {0x01, 0x24, 0x25, 0x02, 0x41, 0x09, 0xC4};
    cmd->onExecuteResult(ExecuteResult::SUCCESS, payload);

    EXPECT_EQ(received.result, CommandResult::Result::OK);
    EXPECT_EQ(received.register_id, 0x0124);
    EXPECT_NEAR(received.value, 250.0, 0.01);
    EXPECT_EQ(received.unit, KmpUnit::C);
}

TEST_F(GetRegisterCommandTest, DecodeBatteryLife_WrongUnitBug) {
    auto cmd = std::make_shared<GetRegisterCommand>(mockApp, 0x0246);
    CommandResult received;
    cmd->registerListener([&](const CommandResult& r) { received = r; });
    std::vector<uint8_t> payload = {0x02, 0x46, 0x3E, 0x02, 0x43, 0x00, 0x17};
    cmd->onExecuteResult(ExecuteResult::SUCCESS, payload);

    EXPECT_EQ(received.result, CommandResult::Result::OK);
    EXPECT_EQ(received.register_id, 0x0246);
    EXPECT_NEAR(received.value, 0.023, 0.001);
    EXPECT_EQ(received.unit, KmpUnit::M3);
}

TEST_F(GetRegisterCommandTest, UnknownUnit) {
    auto cmd = std::make_shared<GetRegisterCommand>(mockApp, 0x0044);
    CommandResult received;
    cmd->registerListener([&](const CommandResult& r) { received = r; });
    std::vector<uint8_t> payload = {0x00, 0x44, 0x28, 0x04, 0x43, 0x00, 0x00, 0x15, 0xC0};
    cmd->onExecuteResult(ExecuteResult::SUCCESS, payload);

    EXPECT_EQ(received.result, CommandResult::Result::OK);
    EXPECT_NEAR(received.value, 5.568, 0.001);
    EXPECT_EQ(received.unit, static_cast<KmpUnit>(0x28));
    EXPECT_EQ(unitToString(received.unit), "UNKNOWN");
}

TEST_F(GetRegisterCommandTest, NoB_Zero_Unsupported) {
    auto cmd = std::make_shared<GetRegisterCommand>(mockApp, 0x0044);
    CommandResult received;
    cmd->registerListener([&](const CommandResult& r) { received = r; });
    std::vector<uint8_t> payload = {0x00, 0x44, 0x38, 0x00, 0x43, 0x00, 0x00, 0x15, 0xC0};
    cmd->onExecuteResult(ExecuteResult::SUCCESS, payload);

    EXPECT_EQ(received.result, CommandResult::Result::CORRUPT);
}

TEST_F(GetRegisterCommandTest, NoB_GreaterThanMax_Unsupported) {
    auto cmd = std::make_shared<GetRegisterCommand>(mockApp, 0x0044);
    CommandResult received;
    cmd->registerListener([&](const CommandResult& r) { received = r; });
    std::vector<uint8_t> payload = {0x00, 0x44, 0x38, 0x09, 0x43, 0x00, 0x00, 0x15, 0xC0};
    cmd->onExecuteResult(ExecuteResult::SUCCESS, payload);

    EXPECT_EQ(received.result, CommandResult::Result::CORRUPT);
}
