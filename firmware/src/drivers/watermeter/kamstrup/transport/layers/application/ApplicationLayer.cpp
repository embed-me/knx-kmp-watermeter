#include "ApplicationLayer.hpp"
#include "src/drivers/logger/Logger.hpp"

using namespace drivers::logger;

namespace drivers::watermeter::kamstrup::transport {

ApplicationLayer::ApplicationLayer(std::shared_ptr<IDataLinkLayer> dl)
    : dataLinkLayer(dl) 
{
    if (dataLinkLayer) {
        dataLinkLayer->registerListener([this](const std::vector<uint8_t>& f){ this->onDataLinkReceive(f); });
        dataLinkLayer->registerAckListener([this](AckType t){ this->onDataLinkAck(t); });
    }
}

void ApplicationLayer::sendRequest(uint8_t cid, const std::vector<uint8_t>& payload) 
{
    std::vector<uint8_t> msg;
    msg.reserve(sizeof(cid) + payload.size());
    msg.push_back(cid);
    msg.insert(msg.end(), payload.begin(), payload.end());
    if (!dataLinkLayer) {
        logError("ApplicationLayer: data link not configured");
        return;
    }
    dataLinkLayer->sendApplicationPayload(msg);
}

void ApplicationLayer::registerHandler(uint8_t cid, std::function<void(const std::vector<uint8_t>&)> handler) 
{
    handlers_[cid].push_back(handler);
}

void ApplicationLayer::onDataLinkReceive(const std::vector<uint8_t>& appFrame) 
{
    if (appFrame.empty()) {
        logWarning("ApplicationLayer: empty frame received");
        return;
    }

    uint8_t cid = appFrame[0];
    std::vector<uint8_t> payload;
    if (appFrame.size() > 1) {
        payload.assign(appFrame.begin() + sizeof(cid), appFrame.end());
    }

    auto it = handlers_.find(cid);
    if (it != handlers_.end()) {
        for (auto &h : it->second) {
            h(payload);
        }
    } else {
        logInfo("ApplicationLayer: no handler for CID 0x%02X", cid);
    }
}

void ApplicationLayer::registerAckListener(std::function<void(AckType)> cb) 
{
    ackListeners_.push_back(cb);
}

void ApplicationLayer::onDataLinkAck(AckType type) 
{
    notifyAckListeners(type);
}

void ApplicationLayer::notifyAckListeners(AckType type) 
{
    for (auto &l : ackListeners_) {
        l(type);
    }
}

} // namespace

