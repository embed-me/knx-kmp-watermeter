#ifndef _I_PHYSICAL_LAYER_HPP_
#define _I_PHYSICAL_LAYER_HPP_

#include <cstdint>
#include <vector>
#include <functional>

#include "../types.hpp"

namespace drivers::watermeter::kamstrup::transport {

class IPhysicalLayer {
public:
    virtual ~IPhysicalLayer() = default;

    // Sends a raw data-link payload (destination + CID + data + CRC).
    virtual void sendFrame(const std::vector<uint8_t>& data) = 0;

    // Register to receive unframed data-link payloads from the physical layer.
    virtual void registerListener(std::function<void(const std::vector<uint8_t>&)> cb) = 0;

    // Register to receive out-of-band ACK / NAK from the physical layer.
    virtual void registerAckListener(std::function<void(AckType)> cb) = 0;
};

}

#endif //_I_PHYSICAL_LAYER_HPP_
