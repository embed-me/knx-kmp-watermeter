#include "CRC.hpp"

namespace drivers::watermeter::kamstrup::transport::utils {

uint16_t crc16_ccitt(const uint8_t* data, size_t len) {
    uint16_t crc = 0x0000;
    const uint16_t poly = 0x1021;
    for (size_t i = 0; i < len; ++i) {
        crc ^= static_cast<uint16_t>(data[i]) << 8;
        for (int b = 0; b < 8; ++b) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ poly;
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}

uint16_t crc16_ccitt(const std::vector<uint8_t>& data) {
    if (data.empty()) {
        return 0;
    }
    return crc16_ccitt(data.data(), data.size());
}

} // namespace
