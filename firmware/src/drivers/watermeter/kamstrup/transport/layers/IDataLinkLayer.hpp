#ifndef _I_DATA_LINK_LAYER_HPP_
#define _I_DATA_LINK_LAYER_HPP_

#include <vector>
#include <functional>
#include <cstdint>

#include "../types.hpp"

namespace drivers::watermeter::kamstrup::transport {

class IDataLinkLayer {
public:
    virtual ~IDataLinkLayer() = default;

    // Send application payload (CID + payload bytes). The data link layer will add destination and CRC.
    virtual void sendApplicationPayload(const std::vector<uint8_t>& appPayload) = 0;

    // Register to receive application payloads from data link layer (CID + payload bytes).
    virtual void registerListener(std::function<void(const std::vector<uint8_t>&)> cb) = 0;

    // Register to receive out-of-band ACK / NAK.
    virtual void registerAckListener(std::function<void(AckType)> cb) = 0;
};

}

#endif //_I_DATA_LINK_LAYER_HPP_
