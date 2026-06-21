#ifndef _MOCK_APPLICATION_LAYER_HPP_
#define _MOCK_APPLICATION_LAYER_HPP_

#include "src/drivers/watermeter/kamstrup/transport/layers/IApplicationLayer.hpp"
#include <functional>
#include <vector>
#include <cstdint>

using namespace drivers::watermeter::kamstrup::transport;

struct MockApplicationLayer : public IApplicationLayer {
    uint8_t lastCid = 0xFF;
    std::vector<uint8_t> lastPayload;
    bool sendRequestCalled = false;

    void sendRequest(uint8_t cid, const std::vector<uint8_t>& payload) override {
        lastCid = cid;
        lastPayload = payload;
        sendRequestCalled = true;
    }

    void registerHandler(uint8_t, std::function<void(const std::vector<uint8_t>&)>) override {}
    void registerAckListener(std::function<void(AckType)>) override {}
};

#endif //_MOCK_APPLICATION_LAYER_HPP_
