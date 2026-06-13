#ifndef _GET_SERIAL_NUMBER_CMD_HPP_
#define _GET_SERIAL_NUMBER_CMD_HPP_

#include "ICommand.hpp"
#include "../transport/layers/IApplicationLayer.hpp"

#include <memory>
#include <vector>

namespace drivers::watermeter::kamstrup::transport {

class GetSerialNumberCommand : public ICommand, public std::enable_shared_from_this<GetSerialNumberCommand> {
public:
    static constexpr uint8_t CID = 0x02;

    GetSerialNumberCommand(std::shared_ptr<IApplicationLayer> app);
    virtual ~GetSerialNumberCommand() = default;

    uint8_t getCid() const override { return CID; }
    void execute() override;
    void registerListener(std::function<void(const CommandResult&)> cb) override;
    void onResult(const CommandResult& res) override;

private:
    void onResponse(const std::vector<uint8_t>& payload);

    std::shared_ptr<IApplicationLayer> app_;
    std::vector<std::function<void(const CommandResult&)>> listeners_;
};

}

#endif //_GET_SERIAL_NUMBER_CMD_HPP_
