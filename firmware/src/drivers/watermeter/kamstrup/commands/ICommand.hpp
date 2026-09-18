#ifndef _I_COMMAND_HPP_
#define _I_COMMAND_HPP_

#include "../utils/KmpUnits.hpp"
#include <functional>
#include <string>
#include <cstdint>

namespace drivers::watermeter::kamstrup::transport {

enum class ExecuteResult : uint8_t;

struct CommandResult {
    enum class Result : uint8_t {
        OK = 0,
        TIMEOUT = 1,
        CORRUPT = 2,
        EMPTY = 3,
        ERROR = 4
    };

    Result result{Result::ERROR};
    KmpUnit unit{KmpUnit::UNSPECIFIED};
    double value{0.0};
    std::string value_str; // for strings like serial number
    uint16_t register_id{0}; // optional
};

class ICommand {
public:
    virtual ~ICommand() = default;
    virtual uint8_t getCid() const = 0;
    virtual void execute() = 0;
    virtual void registerListener(std::function<void(const CommandResult&)> cb) = 0;
    virtual void onExecuteResult(ExecuteResult result, const std::vector<uint8_t>& payload) = 0;
};

}

#endif //_I_COMMAND_HPP_
