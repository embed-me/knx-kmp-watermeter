#include <gtest/gtest.h>
#include "src/drivers/watermeter/kamstrup/commands/GetSerialNumberCommand.hpp"
#include "src/drivers/watermeter/kamstrup/utils/CommandQueue.hpp"
#include "../mocks/mock_application_layer.hpp"

#include "src/drivers/watermeter/kamstrup/commands/GetSerialNumberCommand.cpp"

using namespace drivers::watermeter::kamstrup::transport;

class GetSerialNumberCommandTest : public ::testing::Test {
protected:
    void SetUp() override {
        mockApp = std::make_shared<MockApplicationLayer>();
    }

    std::shared_ptr<MockApplicationLayer> mockApp;
};

TEST_F(GetSerialNumberCommandTest, ExecuteSendsCorrectCidAndEmptyPayload) {
    auto cmd = std::make_shared<GetSerialNumberCommand>(mockApp);
    cmd->execute();

    EXPECT_TRUE(mockApp->sendRequestCalled);
    EXPECT_EQ(mockApp->lastCid, GetSerialNumberCommand::CID);
    EXPECT_TRUE(mockApp->lastPayload.empty());
}

TEST_F(GetSerialNumberCommandTest, ExecuteWithNullApp_NoCrash) {
    auto cmd = std::make_shared<GetSerialNumberCommand>(nullptr);
    EXPECT_NO_THROW(cmd->execute());
}

TEST_F(GetSerialNumberCommandTest, OnResultTimeout) {
    auto cmd = std::make_shared<GetSerialNumberCommand>(mockApp);
    CommandResult received;
    cmd->registerListener([&](const CommandResult& r) { received = r; });
    cmd->onExecuteResult(ExecuteResult::TIMEOUT, {});

    EXPECT_EQ(received.result, CommandResult::Result::TIMEOUT);
}

TEST_F(GetSerialNumberCommandTest, OnResultPrintableSerial) {
    auto cmd = std::make_shared<GetSerialNumberCommand>(mockApp);
    CommandResult received;
    cmd->registerListener([&](const CommandResult& r) { received = r; });
    std::vector<uint8_t> payload = {0x30, 0x31, 0x39, 0x46, 0x34, 0x34, 0x31, 0x36};
    cmd->onExecuteResult(ExecuteResult::SUCCESS, payload);

    EXPECT_EQ(received.result, CommandResult::Result::OK);
    EXPECT_EQ(received.value_str, "019F4416");
}

TEST_F(GetSerialNumberCommandTest, OnResultNonPrintableHex) {
    auto cmd = std::make_shared<GetSerialNumberCommand>(mockApp);
    CommandResult received;
    cmd->registerListener([&](const CommandResult& r) { received = r; });
    cmd->onExecuteResult(ExecuteResult::SUCCESS, {0x00, 0xFF, 0x01});

    EXPECT_EQ(received.result, CommandResult::Result::OK);
    EXPECT_EQ(received.value_str, "00FF01");
}

TEST_F(GetSerialNumberCommandTest, OnResultEmptyPayload) {
    auto cmd = std::make_shared<GetSerialNumberCommand>(mockApp);
    CommandResult received;
    cmd->registerListener([&](const CommandResult& r) { received = r; });
    cmd->onExecuteResult(ExecuteResult::SUCCESS, {});

    EXPECT_EQ(received.result, CommandResult::Result::EMPTY);
}
