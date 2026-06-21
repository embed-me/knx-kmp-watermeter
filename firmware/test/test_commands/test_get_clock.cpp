#include <gtest/gtest.h>
#include "src/drivers/watermeter/kamstrup/commands/GetClockCommand.hpp"
#include "src/drivers/watermeter/kamstrup/utils/CommandQueue.hpp"
#include "../mocks/mock_application_layer.hpp"

#include "src/drivers/watermeter/kamstrup/commands/GetClockCommand.cpp"

using namespace drivers::watermeter::kamstrup::transport;

class GetClockCommandTest : public ::testing::Test {
protected:
    void SetUp() override {
        mockApp = std::make_shared<MockApplicationLayer>();
    }

    std::shared_ptr<MockApplicationLayer> mockApp;
};

TEST_F(GetClockCommandTest, ExecuteSendsCorrectCidAndEmptyPayload) {
    auto cmd = std::make_shared<GetClockCommand>(mockApp);
    cmd->execute();

    EXPECT_TRUE(mockApp->sendRequestCalled);
    EXPECT_EQ(mockApp->lastCid, GetClockCommand::CID);
    EXPECT_TRUE(mockApp->lastPayload.empty());
}

TEST_F(GetClockCommandTest, ExecuteWithNullApp_NoCrash) {
    auto cmd = std::make_shared<GetClockCommand>(nullptr);
    EXPECT_NO_THROW(cmd->execute());
}

TEST_F(GetClockCommandTest, OnResultTimeout) {
    auto cmd = std::make_shared<GetClockCommand>(mockApp);
    CommandResult received;
    cmd->registerListener([&](const CommandResult& r) { received = r; });
    cmd->onExecuteResult(ExecuteResult::TIMEOUT, {});

    EXPECT_EQ(received.result, CommandResult::Result::TIMEOUT);
}

TEST_F(GetClockCommandTest, OnResult6RawBytes_ClockParse) {
    auto cmd = std::make_shared<GetClockCommand>(mockApp);
    CommandResult received;
    cmd->registerListener([&](const CommandResult& r) { received = r; });
    cmd->onExecuteResult(ExecuteResult::SUCCESS, {0x19, 0x06, 0x15, 0x14, 0x30, 0x00});

    EXPECT_EQ(received.result, CommandResult::Result::OK);
    EXPECT_EQ(received.value_str, "2025-06-21 20:48:00");
}

TEST_F(GetClockCommandTest, OnResult7PrintableBytes_String) {
    auto cmd = std::make_shared<GetClockCommand>(mockApp);
    CommandResult received;
    cmd->registerListener([&](const CommandResult& r) { received = r; });
    cmd->onExecuteResult(ExecuteResult::SUCCESS, {0x32, 0x30, 0x32, 0x35, 0x2D, 0x30, 0x36});

    EXPECT_EQ(received.result, CommandResult::Result::OK);
    EXPECT_EQ(received.value_str, "2025-06");
}

TEST_F(GetClockCommandTest, OnResult3Bytes_HexString) {
    auto cmd = std::make_shared<GetClockCommand>(mockApp);
    CommandResult received;
    cmd->registerListener([&](const CommandResult& r) { received = r; });
    cmd->onExecuteResult(ExecuteResult::SUCCESS, {0x00, 0x01, 0x02});

    EXPECT_EQ(received.result, CommandResult::Result::OK);
    EXPECT_EQ(received.value_str, "000102");
}

TEST_F(GetClockCommandTest, OnResultEmptyPayload) {
    auto cmd = std::make_shared<GetClockCommand>(mockApp);
    CommandResult received;
    cmd->registerListener([&](const CommandResult& r) { received = r; });
    cmd->onExecuteResult(ExecuteResult::SUCCESS, {});

    EXPECT_EQ(received.result, CommandResult::Result::EMPTY);
}
