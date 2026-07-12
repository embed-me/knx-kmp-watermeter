#ifndef _MOCK_COMMAND_HPP_
#define _MOCK_COMMAND_HPP_

#include "src/drivers/watermeter/kamstrup/commands/ICommand.hpp"
#include <functional>
#include <vector>
#include <cstdint>

using namespace drivers::watermeter::kamstrup::transport;

struct MockCommand : public ICommand {
    uint8_t cid = 0x10;
    bool executeCalled = false;
    std::function<void(const CommandResult&)> listener;
    std::vector<uint8_t> lastPayload;
    ExecuteResult lastResult = ExecuteResult::TIMEOUT;

    uint8_t getCid() const override { return cid; }

    void execute() override { executeCalled = true; }

    void registerListener(std::function<void(const CommandResult&)> cb) override {
        listener = cb;
    }

    void onExecuteResult(ExecuteResult result, const std::vector<uint8_t>& payload) override {
        lastResult = result;
        lastPayload = payload;
        if (listener) {
            CommandResult cr;
            cr.result = (result == ExecuteResult::SUCCESS)
                ? CommandResult::Result::OK : CommandResult::Result::TIMEOUT;
            listener(cr);
        }
    }

    void reset() {
        executeCalled = false;
        lastPayload.clear();
        lastResult = ExecuteResult::TIMEOUT;
    }

    static std::shared_ptr<MockCommand> create(uint8_t cid = 0x10) {
        auto cmd = std::make_shared<MockCommand>();
        cmd->cid = cid;
        return cmd;
    }
};

#endif //_MOCK_COMMAND_HPP_
