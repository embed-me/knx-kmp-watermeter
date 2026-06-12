#ifndef _KMP_UNITS_HPP_
#define _KMP_UNITS_HPP_

#include <cstdint>
#include <string>

namespace drivers::watermeter::kamstrup::transport {

enum class KmpUnit : uint8_t {
    UNSPECIFIED = 0,
    Wh = 1,
    kWh = 2,
    MWh = 3,
    GWh = 4,
    J = 5,
    kJ = 6,
    MJ = 7,
    GJ = 8,
    ASCII = 50,
    L = 32,
    M3 = 33,
    L_PER_H = 34,
    UNKNOWN = 255
};

inline std::string unitToString(KmpUnit u) {
    switch(u) {
        case KmpUnit::UNSPECIFIED: return "";
        case KmpUnit::Wh: return "Wh";
        case KmpUnit::kWh: return "kWh";
        case KmpUnit::MWh: return "MWh";
        case KmpUnit::GWh: return "GWh";
        case KmpUnit::J: return "J";
        case KmpUnit::kJ: return "kJ";
        case KmpUnit::MJ: return "MJ";
        case KmpUnit::GJ: return "GJ";
        case KmpUnit::ASCII: return "ASCII";
        case KmpUnit::L: return "l";
        case KmpUnit::M3: return "m3";
        case KmpUnit::L_PER_H: return "l/h";
        default: return "UNKNOWN";
    }
}

} // namespace

#endif //_KMP_UNITS_HPP_
