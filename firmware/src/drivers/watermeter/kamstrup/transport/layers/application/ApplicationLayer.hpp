#ifndef _APPLICATION_LAYER_HPP_
#define _APPLICATION_LAYER_HPP_

#include "../IApplicationLayer.hpp"
#include "../IDataLinkLayer.hpp"

#include <vector>
#include <functional>
#include <memory>
#include <unordered_map>
#include <cstdint>

namespace drivers::watermeter::kamstrup::transport {

class ApplicationLayer : public IApplicationLayer, public std::enable_shared_from_this<ApplicationLayer> {
public:
    ApplicationLayer(std::shared_ptr<IDataLinkLayer> dl);
    ~ApplicationLayer() = default;

    void sendRequest(uint8_t cid, const std::vector<uint8_t>& payload) override;
    void registerHandler(uint8_t cid, std::function<void(const std::vector<uint8_t>&)> handler) override;
    void registerAckListener(std::function<void(AckType)> cb) override;

private:
    void onDataLinkReceive(const std::vector<uint8_t>& appFrame);
    void onDataLinkAck(AckType type);
    void notifyAckListeners(AckType type);

    std::shared_ptr<IDataLinkLayer> dataLinkLayer;
    std::unordered_map<uint8_t, std::vector<std::function<void(const std::vector<uint8_t>&)>>> handlers_;
    std::vector<std::function<void(AckType)>> ackListeners_;
};

}

#endif //_APPLICATION_LAYER_HPP_
