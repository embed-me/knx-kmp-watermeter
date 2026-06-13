#ifndef _GET_REGISTER_CMD_HPP_
#define _GET_REGISTER_CMD_HPP_

#include "ICommand.hpp"
#include "../transport/layers/IApplicationLayer.hpp"

#include <memory>
#include <vector>

namespace drivers::watermeter::kamstrup::transport {

class GetRegisterCommand : public ICommand, public std::enable_shared_from_this<GetRegisterCommand> {
public:
    static constexpr uint8_t CID = 0x10;

    GetRegisterCommand(std::shared_ptr<IApplicationLayer> app, uint16_t registerId);
    virtual ~GetRegisterCommand() = default;

    uint8_t getCid() const override { return CID; }
    void execute() override;
    void registerListener(std::function<void(const CommandResult&)> cb) override;
    void onResult(const CommandResult& res) override;

private:
    void onResponse(const std::vector<uint8_t>& payload);
    double pow10_int(int8_t exp);

    std::shared_ptr<IApplicationLayer> app_;
    uint16_t register_id_;
    std::vector<std::function<void(const CommandResult&)>> listeners_;
};

}

#endif //_GET_REGISTER_CMD_HPP_
