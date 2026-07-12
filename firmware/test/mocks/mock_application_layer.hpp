#ifndef _MOCK_APPLICATION_LAYER_HPP_
#define _MOCK_APPLICATION_LAYER_HPP_

#include "src/drivers/watermeter/kamstrup/transport/layers/IApplicationLayer.hpp"
#include <functional>
#include <vector>
#include <cstdint>
#include <map>

using namespace drivers::watermeter::kamstrup::transport;

struct MockApplicationLayer : public IApplicationLayer {
    uint8_t lastCid = 0xFF;
    std::vector<uint8_t> lastPayload;
    bool sendRequestCalled = false;
    std::map<uint8_t, std::function<void(const std::vector<uint8_t>&)>> handlers;

    void sendRequest(uint8_t cid, const std::vector<uint8_t>& payload) override {
        lastCid = cid;
        lastPayload = payload;
        sendRequestCalled = true;
    }

    void registerHandler(uint8_t cid, std::function<void(const std::vector<uint8_t>&)> handler) override {
        handlers[cid] = handler;
    }

    void registerAckListener(std::function<void(AckType)>) override {}

    void invokeHandler(uint8_t cid, const std::vector<uint8_t>& payload) {
        auto it = handlers.find(cid);
        if (it != handlers.end()) {
            it->second(payload);
        }
    }

    void reset() {
        handlers.clear();
        sendRequestCalled = false;
        lastCid = 0xFF;
        lastPayload.clear();
    }
};

#endif //_MOCK_APPLICATION_LAYER_HPP_
