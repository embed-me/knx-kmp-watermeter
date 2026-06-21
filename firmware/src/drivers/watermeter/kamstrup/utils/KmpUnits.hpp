#ifndef _KMP_UNITS_HPP_
#define _KMP_UNITS_HPP_

#include <cstdint>
#include <string>

namespace drivers::watermeter::kamstrup::transport {

enum class KmpUnit : uint8_t {
    UNSPECIFIED = 0,
    C = 0x25,
    L = 0x27,
    L_PER_H = 0x29,
    H = 0x2E,
    CLOCK = 0x2F,
    M3 = 0x3E,
    UNKNOWN = 0xFF
};

inline std::string unitToString(KmpUnit u) {
    switch(u) {
        case KmpUnit::UNSPECIFIED: return "";
        case KmpUnit::C: return "C";
        case KmpUnit::L: return "l";
        case KmpUnit::M3: return "m3";
        case KmpUnit::L_PER_H: return "l/h";
        case KmpUnit::H: return "h";
        case KmpUnit::CLOCK: return "clock";
        default: return "UNKNOWN";
    }
}

} // namespace

#endif //_KMP_UNITS_HPP_
