#ifndef _MOCK_PHYSICAL_LAYER_HPP_
#define _MOCK_PHYSICAL_LAYER_HPP_

#include "src/drivers/watermeter/kamstrup/transport/layers/IPhysicalLayer.hpp"
#include <functional>
#include <vector>
#include <cstdint>

using namespace drivers::watermeter::kamstrup::transport;

struct MockPhysicalLayer : public IPhysicalLayer {
    std::vector<uint8_t> lastFrame;

    void sendFrame(const std::vector<uint8_t>& data) override {
        lastFrame = data;
    }

    void registerListener(std::function<void(const std::vector<uint8_t>&)> cb) override {
        dataListener = cb;
    }

    void registerAckListener(std::function<void(AckType)> cb) override {
        ackListener = cb;
    }

    void simulateFrame(const std::vector<uint8_t>& data) {
        if (dataListener) dataListener(data);
    }

    void simulateAck(AckType type) {
        if (ackListener) ackListener(type);
    }

    std::function<void(const std::vector<uint8_t>&)> dataListener;
    std::function<void(AckType)> ackListener;
};

#endif //_MOCK_PHYSICAL_LAYER_HPP_
