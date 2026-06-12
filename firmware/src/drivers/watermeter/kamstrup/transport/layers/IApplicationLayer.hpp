#ifndef _I_APPLICATION_LAYER_HPP_
#define _I_APPLICATION_LAYER_HPP_

#include <vector>
#include <functional>
#include <cstdint>

#include "../types.hpp"

namespace drivers::watermeter::kamstrup::transport {

class IApplicationLayer {
public:
    virtual ~IApplicationLayer() = default;

    // Send a request with CID and payload (payload excludes the CID byte).
    virtual void sendRequest(uint8_t cid, const std::vector<uint8_t>& payload) = 0;

    // Register a handler for a specific CID; handler receives payload bytes excluding CID.
    virtual void registerHandler(uint8_t cid, std::function<void(const std::vector<uint8_t>&)> handler) = 0;

    // Register to receive out-of-band ACK / NAK.
    virtual void registerAckListener(std::function<void(AckType)> cb) = 0;
};

}

#endif //_I_APPLICATION_LAYER_HPP_
