#ifndef _PHYSICAL_LAYER_HPP_
#define _PHYSICAL_LAYER_HPP_

#include "../IPhysicalLayer.hpp"
#include "src/drivers/uart/IUartDriver.hpp"
#include "../../../utils/ByteStuff.hpp"

#include <vector>
#include <functional>
#include <memory>

namespace drivers::watermeter::kamstrup::transport {

class PhysicalLayer : public IPhysicalLayer, public std::enable_shared_from_this<PhysicalLayer> {
public:
    PhysicalLayer(std::shared_ptr<drivers::uart::IUartDriver> uart);
    ~PhysicalLayer() = default;

    void sendFrame(const std::vector<uint8_t>& data) override;
    void registerListener(std::function<void(const std::vector<uint8_t>&)> cb) override;
    void registerAckListener(std::function<void(AckType)> cb) override;

private:
    void onUartData(const std::vector<uint8_t>& chunk);
    void processCompleteFrame();
    bool extractFrameContent(std::vector<uint8_t>& frameContent);
    void handleFrame(const std::vector<uint8_t>& frameContent);
    void notifyListeners(const std::vector<uint8_t>& data);
    void notifyAckListeners(AckType type);

    std::shared_ptr<drivers::uart::IUartDriver> uart_;
    std::vector<std::function<void(const std::vector<uint8_t>&)>> listeners_;
    std::vector<std::function<void(AckType)>> ackListeners_;

    // Receive buffer for assembling frames
    std::vector<uint8_t> rx_buffer_;

    // Track whether we are inside a framed response (between START_TO_HOST and STOP).
    // ACK/NAK bytes are only interpreted outside a frame; inside, they are valid data.
    bool inside_frame_ = false;

    const uint8_t START_TO_METER = 0x80;
    const uint8_t START_TO_HOST = 0x40;
    const uint8_t STOP = 0x0D;
    const uint8_t ESC = 0x1B;
    const uint8_t ACK = 0x06;
    const uint8_t NAK = 0x15;
};

}

#endif //_PHYSICAL_LAYER_HPP_
