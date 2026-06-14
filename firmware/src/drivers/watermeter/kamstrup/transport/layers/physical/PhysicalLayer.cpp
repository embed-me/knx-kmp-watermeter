#include "PhysicalLayer.hpp"
#include "src/drivers/logger/Logger.hpp"
#include <algorithm>

using namespace drivers::logger;

namespace drivers::watermeter::kamstrup::transport {

PhysicalLayer::PhysicalLayer(std::shared_ptr<drivers::uart::IUartDriver> uart)
    : uart_(uart)
{
    if (uart_) {
        uart_->setRxCallback([this](const std::vector<uint8_t>& chunk){ this->onUartData(chunk); });
    }
}

void PhysicalLayer::registerListener(std::function<void(const std::vector<uint8_t>&)> cb) 
{
    listeners_.push_back(cb);
}

void PhysicalLayer::sendFrame(const std::vector<uint8_t>& data) 
{
    std::vector<uint8_t> tx;
    tx.reserve(data.size());
    tx.push_back(START_TO_METER);

    auto stuffed = utils::stuff_bytes(data);
    tx.insert(tx.end(), stuffed.begin(), stuffed.end());

    tx.push_back(STOP);

    if (!uart_) {
        logError("PhysicalLayer: UART not configured");
        return;
    }

    int written = uart_->write(tx);
    if (written < 0) {
        logError("PhysicalLayer: UART write failed");
    } else {
        logTrace("PhysicalLayer: wrote %d bytes", written);
    }
}

void PhysicalLayer::onUartData(const std::vector<uint8_t>& chunk) 
{
    for (auto b : chunk) {
        if (!inside_frame_ && b == ACK) {
            notifyAckListeners(AckType::Ack);
            continue;
        }
        if (!inside_frame_ && b == NAK) {
            notifyAckListeners(AckType::Nak);
            continue;
        }
        rx_buffer_.push_back(b);
        if (b == START_TO_HOST) {
            inside_frame_ = true;
        }
        if (b == STOP) {
            inside_frame_ = false;
            processCompleteFrame();
        }
    }
}

void PhysicalLayer::processCompleteFrame() 
{
    std::vector<uint8_t> frameContent;
    if (!extractFrameContent(frameContent)) {
        rx_buffer_.clear();
        return;
    }
    handleFrame(frameContent);
    rx_buffer_.clear();
}

bool PhysicalLayer::extractFrameContent(std::vector<uint8_t>& frameContent) 
{
    if (rx_buffer_.empty()) {
        return false;
    }

    auto it = std::find_if(rx_buffer_.begin(), rx_buffer_.end(), [this](uint8_t x){ return x == START_TO_HOST; });
    if (it == rx_buffer_.end()) {
        return false;
    }

    size_t start_idx = std::distance(rx_buffer_.begin(), it);
    if (rx_buffer_.size() <= start_idx + 1) {
        return false;
    }

    frameContent.assign(rx_buffer_.begin() + start_idx, rx_buffer_.end());
    return true;
}

void PhysicalLayer::handleFrame(const std::vector<uint8_t>& frameContent) 
{
    if (frameContent.empty() || frameContent.front() != START_TO_HOST) {
        logWarning("PhysicalLayer: received frame without expected START_TO_HOST");
        return;
    }

    if (frameContent.size() < 2) {
        logWarning("PhysicalLayer: frame too short");
        return;
    }

    std::vector<uint8_t> inner(frameContent.begin() + 1, frameContent.end() - 1);
    auto unstuffed = utils::unstuff_bytes(inner);
    notifyListeners(unstuffed);
}

void PhysicalLayer::notifyListeners(const std::vector<uint8_t>& data) 
{
    for (auto &l : listeners_) {
        l(data);
    }
}

void PhysicalLayer::registerAckListener(std::function<void(AckType)> cb) 
{
    ackListeners_.push_back(cb);
}

void PhysicalLayer::notifyAckListeners(AckType type) 
{
    for (auto &l : ackListeners_) {
        l(type);
    }
}

} // namespace

