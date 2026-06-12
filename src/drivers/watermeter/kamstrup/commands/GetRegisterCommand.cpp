#include "GetRegisterCommand.hpp"
#include "src/drivers/logger/Logger.hpp"
#include <cmath>

using namespace drivers::logger;

namespace drivers::watermeter::kamstrup::transport {

static constexpr uint8_t KMP_READ_REQUEST_PREFIX = 0x01;
static constexpr uint8_t EXPONENT_MASK = 0x3F;
static constexpr uint8_t EXPONENT_SIGN_BIT = 0x40;
static constexpr uint8_t EXPONENT_NEGATE_BIT = 0x80;
static constexpr uint8_t MIN_RESPONSE_SIZE = 4;
static constexpr uint8_t MIN_RESPONSE_WITH_PREFIX_SIZE = 5;
static constexpr uint8_t UNIT_IDX_WITH_PREFIX = 3;
static constexpr uint8_t UNIT_IDX_NO_PREFIX = 2;
static constexpr uint8_t MAX_MANTISSA_RANGE = 4;

GetRegisterCommand::GetRegisterCommand(std::shared_ptr<IApplicationLayer> app, uint16_t registerId)
    : app_(app), register_id_(registerId) 
{
    if (app_) {
        app_->registerHandler(CID, [this](const std::vector<uint8_t>& p){ this->onResponse(p); });
    }
}

void GetRegisterCommand::execute() 
{
    if (!app_) {
        logError("GetRegisterCommand: application layer unavailable");
        return;
    }
    std::vector<uint8_t> payload;
    payload.push_back(KMP_READ_REQUEST_PREFIX);
    payload.push_back(static_cast<uint8_t>((register_id_ >> 8) & 0xFF));
    payload.push_back(static_cast<uint8_t>(register_id_ & 0xFF));
    app_->sendRequest(CID, payload);
}

void GetRegisterCommand::registerListener(std::function<void(const CommandResult&)> cb)
{
    listeners_.push_back(cb);
}

double GetRegisterCommand::pow10_int(int8_t exp) 
{
    return std::pow(10.0, static_cast<double>(exp));
}

void GetRegisterCommand::onResponse(const std::vector<uint8_t>& payload) 
{
    CommandResult res;

    if (payload.empty()) {
        res.success = false;
        logWarning("GetRegisterCommand: empty payload");
    } else {
        bool has_prefix = (payload[0] == KMP_READ_REQUEST_PREFIX);
        uint8_t min_size = has_prefix ? MIN_RESPONSE_WITH_PREFIX_SIZE : MIN_RESPONSE_SIZE;

        if (payload.size() < min_size) {
            res.success = false;
            logWarning("GetRegisterCommand: payload too small (%u bytes)", (uint16_t)payload.size());
        } else {
            uint8_t cmd_idx = has_prefix ? 1 : 0;
            uint8_t unit_idx = has_prefix ? UNIT_IDX_WITH_PREFIX : UNIT_IDX_NO_PREFIX;

            if (has_prefix) {
                uint16_t recv_reg_id = (static_cast<uint16_t>(payload[cmd_idx]) << 8) | payload[cmd_idx + 1];
                if (recv_reg_id != register_id_) {
                    logWarning("GetRegisterCommand: register ID mismatch (got 0x%04X expected 0x%04X)", recv_reg_id, register_id_);
                }
            }

            uint8_t unit_byte = payload[unit_idx];
            uint8_t mantissa_range = payload[unit_idx + 1];
            uint8_t exponent_b = payload[unit_idx + 2];

            if (mantissa_range > MAX_MANTISSA_RANGE) {
                res.success = false;
                logError("GetRegisterCommand: invalid mantissa range %d", mantissa_range);
            } else if (unit_idx + 3 + mantissa_range > payload.size()) {
                res.success = false;
                logError("GetRegisterCommand: payload too short for mantissa");
            } else {
                int8_t exp_val = static_cast<int8_t>(exponent_b & EXPONENT_MASK);
                if (exponent_b & EXPONENT_SIGN_BIT) {
                    exp_val = -exp_val;
                }

                uint64_t mantissa = 0;
                for (uint8_t i = 0; i < mantissa_range; ++i) {
                    mantissa = (mantissa << 8) | payload[unit_idx + 3 + i];
                }

                double value = static_cast<double>(mantissa) * pow10_int(exp_val);
                if (exponent_b & EXPONENT_NEGATE_BIT) {
                    value = -value;
                }

                res.success = true;
                res.value = value;
                res.unit = static_cast<KmpUnit>(unit_byte);
                res.register_id = register_id_;

                logTrace("GetRegisterCommand: decoded register 0x%04X = %f %s",
                         register_id_, value, unitToString(res.unit).c_str());
            }
        }
    }

    for (auto &l : listeners_) {
        l(res);
    }
}

}

