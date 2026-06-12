#ifndef _DATA_LINK_LAYER_HPP_
#define _DATA_LINK_LAYER_HPP_

#include "../IDataLinkLayer.hpp"
#include "../IPhysicalLayer.hpp"
#include "../../../utils/CRC.hpp"

#include <vector>
#include <functional>
#include <memory>
#include <cstdint>

namespace drivers::watermeter::kamstrup::transport {

class DataLinkLayer : public IDataLinkLayer, public std::enable_shared_from_this<DataLinkLayer> {
public:
    DataLinkLayer(std::shared_ptr<IPhysicalLayer> phy, uint8_t destination = 0x3F);
    ~DataLinkLayer() = default;

    void sendApplicationPayload(const std::vector<uint8_t>& appPayload) override;
    void registerListener(std::function<void(const std::vector<uint8_t>&)> cb) override;
    void registerAckListener(std::function<void(AckType)> cb) override;

private:
    void onPhysicalFrame(const std::vector<uint8_t>& frame);
    void onPhysicalAck(AckType type);
    void notifyAckListeners(AckType type);

    std::shared_ptr<IPhysicalLayer> physical_;
    uint8_t destination_;
    std::vector<std::function<void(const std::vector<uint8_t>&)>> listeners_;
    std::vector<std::function<void(AckType)>> ackListeners_;
};

}

#endif //_DATA_LINK_LAYER_HPP_
