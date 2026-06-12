#include "ByteStuff.hpp"

namespace drivers::watermeter::kamstrup::transport::utils {

static const uint8_t ESC = 0x1B;
static const uint8_t RESERVED[] = {0x06, 0x0D, 0x1B, 0x40, 0x80};

bool is_reserved(uint8_t b) {
    for (uint8_t r : RESERVED) {
        if (r == b) {
            return true;
        }
    }
    return false;
}

std::vector<uint8_t> stuff_bytes(const std::vector<uint8_t>& data) {
    std::vector<uint8_t> out;
    for (auto b : data) {
        if (is_reserved(b)) {
            out.push_back(ESC);
            out.push_back(static_cast<uint8_t>(b ^ 0xFF));
        } else {
            out.push_back(b);
        }
    }
    return out;
}

std::vector<uint8_t> unstuff_bytes(const std::vector<uint8_t>& data) {
    std::vector<uint8_t> out;
    for (size_t i = 0; i < data.size(); ++i) {
        uint8_t b = data[i];
        if (b == ESC) {
            if (i + 1 < data.size()) {
                out.push_back(static_cast<uint8_t>(data[i + 1] ^ 0xFF));
                ++i;
            } else {
                // malformed escape at end
                break;
            }
        } else {
            out.push_back(b);
        }
    }
    return out;
}

} // namespace
