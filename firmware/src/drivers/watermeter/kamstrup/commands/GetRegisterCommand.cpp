#include "GetRegisterCommand.hpp"
#include "../utils/CommandQueue.hpp"
#include "src/drivers/logger/Logger.hpp"
#include <cmath>

using namespace drivers::logger;

namespace drivers::watermeter::kamstrup::transport {

struct SiExFields {
    bool SI;      // sign of value (0=positive, 1=negative)
    bool SE;      // sign of exponent (0=positive, 1=negative)
    uint8_t exponent; // exponent value 0-63
};

static SiExFields decodeSiEx(uint8_t raw)
{
    return {
        .SI = static_cast<bool>((raw >> 7) & 1),
        .SE = static_cast<bool>((raw >> 6) & 1),
        .exponent = static_cast<uint8_t>(raw & 0x3F)
    };
}

static double computeFloatingValue(uint64_t integer, const SiExFields& siEx)
{
    double sign = (siEx.SI) ? -1.0 : 1.0;
    int exp = (siEx.SE) ? -(int)siEx.exponent : (int)siEx.exponent;
    return sign * static_cast<double>(integer) * std::pow(10.0, exp);
}

static bool tryParseRegister(const uint8_t*& pos, const uint8_t* end, CommandResult& res)
{
    constexpr uint8_t REGISTER_ID_SIZE = 2;
    constexpr uint8_t REGISTER_FORMAT_SIZE = 3;
    constexpr uint8_t MAX_NOB = 8;

    if ((pos + REGISTER_ID_SIZE + REGISTER_FORMAT_SIZE) > end) {
        logWarning("GetRegisterCommand: register response malformed or truncated");
        return false;
    }

    uint16_t rid = (static_cast<uint16_t>(pos[0]) << 8) | pos[1];
    pos += 2;

    uint8_t unit = *pos++;
    uint8_t NoB = *pos++;

    if (NoB == 0 || NoB > MAX_NOB) {
        logWarning("GetRegisterCommand: register response has unsupported byte length %d", NoB);
        return false;
    }

    if (pos + NoB > end) {
        logWarning("GetRegisterCommand: register response truncated value");
        return false;
    }

    SiExFields siEx = decodeSiEx(*pos++);

    uint64_t integer = 0;
    for (uint8_t i = 0; i < NoB; ++i) {
        integer = (integer << 8) | *pos++;
    }

    res.register_id = rid;
    res.value = computeFloatingValue(integer, siEx);
    res.unit = static_cast<KmpUnit>(unit);
    res.result = CommandResult::Result::OK;

    logTrace("GetRegisterCommand: decoded register 0x%04X = %g %s",
             rid, res.value, unitToString(res.unit).c_str());

    return true;
}

// --

GetRegisterCommand::GetRegisterCommand(std::shared_ptr<IApplicationLayer> app, uint16_t registerId)
    : app_(app), register_id_(registerId) 
{
}

void GetRegisterCommand::execute() 
{
    if (!app_) {
        logError("GetRegisterCommand: application layer unavailable");
        return;
    }
    std::vector<uint8_t> payload;
    payload.push_back(0x01); // only single register requests supported
    payload.push_back(static_cast<uint8_t>((register_id_ >> 8) & 0xFF));
    payload.push_back(static_cast<uint8_t>(register_id_ & 0xFF));
    app_->sendRequest(CID, payload);
}

void GetRegisterCommand::registerListener(std::function<void(const CommandResult&)> cb)
{
    listeners_.push_back(cb);
}

void GetRegisterCommand::notifyListeners(const CommandResult& res)
{
    for (auto &l : listeners_) {
        l(res);
    }
}

void GetRegisterCommand::onExecuteResult(ExecuteResult result, const std::vector<uint8_t>& payload) 
{
    CommandResult res;
    res.register_id = register_id_;

    if (result == ExecuteResult::SUCCESS) {
        const uint8_t* pos = payload.data();
        const uint8_t* end = pos + payload.size();

        bool success = tryParseRegister(pos, end, res);
        if (!success) {
            res.result = CommandResult::Result::CORRUPT;
        }
    } else if (result == ExecuteResult::TIMEOUT) {
        res.result = CommandResult::Result::TIMEOUT;
    } else {
        res.result = CommandResult::Result::ERROR;
    }

    notifyListeners(res);
}

} // namespace
