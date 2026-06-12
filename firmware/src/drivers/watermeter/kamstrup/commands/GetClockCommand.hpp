#ifndef _GET_CLOCK_CMD_HPP_
#define _GET_CLOCK_CMD_HPP_

#include "ICommand.hpp"
#include "../transport/layers/IApplicationLayer.hpp"

#include <memory>
#include <vector>

namespace drivers::watermeter::kamstrup::transport {

class GetClockCommand : public ICommand, public std::enable_shared_from_this<GetClockCommand> {
public:
    static constexpr uint8_t CID = 0x09;

    GetClockCommand(std::shared_ptr<IApplicationLayer> app);
    virtual ~GetClockCommand() = default;

    uint8_t getCid() const override { return CID; }
    void execute() override;
    void registerListener(std::function<void(const CommandResult&)> cb) override;

private:
    void onResponse(const std::vector<uint8_t>& payload);

    std::shared_ptr<IApplicationLayer> app_;
    std::vector<std::function<void(const CommandResult&)>> listeners_;
};

}

#endif //_GET_CLOCK_CMD_HPP_
