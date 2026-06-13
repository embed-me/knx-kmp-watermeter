#include "GetClockCommand.hpp"
#include "src/drivers/logger/Logger.hpp"
#include <ctime>

using namespace drivers::logger;

namespace drivers::watermeter::kamstrup::transport {

GetClockCommand::GetClockCommand(std::shared_ptr<IApplicationLayer> app)
    : app_(app) 
{
}

void GetClockCommand::execute() 
{
    if (!app_) {
        logError("GetClockCommand: application layer unavailable");
        return;
    }
    app_->sendRequest(CID, {});
}

void GetClockCommand::registerListener(std::function<void(const CommandResult&)> cb)
{
    listeners_.push_back(cb);
}

void GetClockCommand::onResult(const CommandResult& res)
{
    for (auto &l : listeners_) {
        l(res);
    }
}

void GetClockCommand::onResponse(const std::vector<uint8_t>& payload) 
{
    CommandResult res;
    bool parsed = false;
    if (payload.size() >= 6) {
        if (payload.size() >= 7) {
            bool printable = true;
            for (auto b : payload) {
                if (b < 0x20 || b > 0x7e) {
                    printable = false;
                    break;
                }
            }
            if (printable) {
                res.value_str = std::string(payload.begin(), payload.end());
                parsed = true;
            }
        }
        if (!parsed && payload.size() >= 6) {
            int yy = payload[0];
            int mm = payload[1];
            int dd = payload[2];
            int hh = payload[3];
            int mi = payload[4];
            int ss = payload[5];
            char buf[64];
            snprintf(buf, sizeof(buf), "20%02d-%02d-%02d %02d:%02d:%02d", yy, mm, dd, hh, mi, ss);
            res.value_str = buf;
            parsed = true;
        }
    } else if (!payload.empty()) {
        bool printable = true;
        for (auto b : payload) {
            if (b < 0x20 || b > 0x7e) {
                printable = false;
                break;
            }
        }
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
        parsed = true;
    }

    res.result = parsed ? CommandResult::Result::OK : CommandResult::Result::EMPTY;
    onResult(res);
}

} // namespace

