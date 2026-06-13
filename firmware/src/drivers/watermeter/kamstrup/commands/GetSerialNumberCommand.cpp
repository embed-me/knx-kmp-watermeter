#include "GetSerialNumberCommand.hpp"
#include "src/drivers/logger/Logger.hpp"
#include <algorithm>

using namespace drivers::logger;

namespace drivers::watermeter::kamstrup::transport {

GetSerialNumberCommand::GetSerialNumberCommand(std::shared_ptr<IApplicationLayer> app)
    : app_(app) 
{
    if (app_) {
        app_->registerHandler(CID, [this](const std::vector<uint8_t>& p){ this->onResponse(p); });
    }
}

void GetSerialNumberCommand::execute() 
{
    if (!app_) {
        logError("GetSerialNumberCommand: application layer unavailable");
        return;
    }
    app_->sendRequest(CID, {});
}

void GetSerialNumberCommand::registerListener(std::function<void(const CommandResult&)> cb)
{
    listeners_.push_back(cb);
}

void GetSerialNumberCommand::onResult(const CommandResult& res)
{
    for (auto &l : listeners_) {
        l(res);
    }
}

void GetSerialNumberCommand::onResponse(const std::vector<uint8_t>& payload) 
{
    CommandResult res;
    if (payload.empty()) {
        res.result = CommandResult::Result::EMPTY;
    } else {
        bool printable = std::all_of(payload.begin(), payload.end(), [](uint8_t b){ return b >= 0x20 && b <= 0x7e; });
        if (printable) {
            res.value_str = std::string(payload.begin(), payload.end());
        } else {
            char buf[3];
            std::string out;
            for (auto b : payload) {
                snprintf(buf, sizeof(buf), "%02X", b);
                out += buf;
            }
            res.value_str = out;
        }
        res.result = CommandResult::Result::OK;
    }

    onResult(res);
}

} // namespace

