#ifndef _MOCK_DATA_LINK_LAYER_HPP_
#define _MOCK_DATA_LINK_LAYER_HPP_

#include "src/drivers/watermeter/kamstrup/transport/layers/IDataLinkLayer.hpp"
#include <functional>
#include <vector>
#include <cstdint>

using namespace drivers::watermeter::kamstrup::transport;

struct MockDataLinkLayer : public IDataLinkLayer {
    std::vector<uint8_t> lastPayload;

    void sendApplicationPayload(const std::vector<uint8_t>& appPayload) override {
        lastPayload = appPayload;
    }

    void registerListener(std::function<void(const std::vector<uint8_t>&)> cb) override {
        dataListener = cb;
    }

    void registerAckListener(std::function<void(AckType)> cb) override {
        ackListener = cb;
    }

    void simulateReceive(const std::vector<uint8_t>& data) {
        if (dataListener) dataListener(data);
    }

    void simulateAck(AckType type) {
        if (ackListener) ackListener(type);
    }

    std::function<void(const std::vector<uint8_t>&)> dataListener;
    std::function<void(AckType)> ackListener;
};

#endif //_MOCK_DATA_LINK_LAYER_HPP_
