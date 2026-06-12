#include "DataLinkLayer.hpp"
#include "src/drivers/logger/Logger.hpp"
#include <algorithm>

using namespace drivers::logger;

namespace drivers::watermeter::kamstrup::transport {

DataLinkLayer::DataLinkLayer(std::shared_ptr<IPhysicalLayer> phy, uint8_t destination)
    : physical_(phy), destination_(destination) 
{
    if (physical_) {
        physical_->registerListener([this](const std::vector<uint8_t>& f){ this->onPhysicalFrame(f); });
        physical_->registerAckListener([this](AckType t){ this->onPhysicalAck(t); });
    }
}

void DataLinkLayer::registerListener(std::function<void(const std::vector<uint8_t>&)> cb) 
{
    listeners_.push_back(cb);
}

void DataLinkLayer::sendApplicationPayload(const std::vector<uint8_t>& appPayload) 
{
    uint16_t crc;
    std::vector<uint8_t> frame;
    frame.reserve(sizeof (destination_) + sizeof(crc) + appPayload.size());
    frame.push_back(destination_);
    frame.insert(frame.end(), appPayload.begin(), appPayload.end());

    crc = utils::crc16_ccitt(frame);
    frame.push_back(static_cast<uint8_t>((crc >> 8) & 0xFF));
    frame.push_back(static_cast<uint8_t>(crc & 0xFF));

    if (!physical_) {
        logError("DataLinkLayer: physical layer missing");
        return;
    }
    physical_->sendFrame(frame);
}

void DataLinkLayer::onPhysicalFrame(const std::vector<uint8_t>& frame) 
{
    if (frame.size() < 3) {
        logWarning("DataLinkLayer: frame too small");
        return;
    }

    size_t len = frame.size();
    uint16_t recv_crc = (static_cast<uint16_t>(frame[len - 2]) << 8) | frame[len - 1];

    std::vector<uint8_t> without_crc(frame.begin(), frame.begin() + (len - 2));
    uint16_t calc = utils::crc16_ccitt(without_crc);
    if (calc != recv_crc) {
        logError("DataLinkLayer: CRC mismatch (calc=0x%04X recv=0x%04X)", calc, recv_crc);
        return;
    }

    uint8_t dest = frame[0];
    if (dest != destination_) {
        logWarning("DataLinkLayer: destination mismatch (got 0x%02X expected 0x%02X)", dest, destination_);
        return;
    }

    std::vector<uint8_t> appPayload(frame.begin() + 1, frame.begin() + (len - 2));
    for (auto &l : listeners_) {
        l(appPayload);
    }
}

void DataLinkLayer::registerAckListener(std::function<void(AckType)> cb) 
{
    ackListeners_.push_back(cb);
}

void DataLinkLayer::onPhysicalAck(AckType type) 
{
    notifyAckListeners(type);
}

void DataLinkLayer::notifyAckListeners(AckType type) 
{
    for (auto &l : ackListeners_) {
        l(type);
    }
}

} // namespace

