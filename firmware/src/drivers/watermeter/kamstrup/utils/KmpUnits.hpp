#ifndef _KMP_UNITS_HPP_
#define _KMP_UNITS_HPP_

#include <cstdint>
#include <string>

namespace drivers::watermeter::kamstrup::transport {

enum class KmpUnit : uint8_t {
    UNSPECIFIED = 0,
    Wh = 0x01,
    kWh = 0x02,
    MWh = 0x03,
    Gj = 0x08,
    Gcal = 0x0C,
    kW = 0x16,
    MW = 0x17,
    C = 0x25,
    K = 0x26,
    L = 0x27,
    M3 = 0x28,
    L_PER_H = 0x29,
    M3_PER_H = 0x2A,
    M3XC = 0x2B,
    TON = 0x2C,
    TON_PER_H = 0x2D,
    H = 0x2E,
    CLOCK = 0x2F,
    DATE1 = 0x30,
    DATE3 = 0x32,
    NUMBER = 0x33,
    BAR = 0x34,
    UNKNOWN = 0xFF
};

inline std::string unitToString(KmpUnit u) {
    switch(u) {
        case KmpUnit::UNSPECIFIED: return "";
        case KmpUnit::Wh: return "Wh";
        case KmpUnit::kWh: return "kWh";
        case KmpUnit::MWh: return "MWh";
        case KmpUnit::Gj: return "Gj";
        case KmpUnit::Gcal: return "Gcal";
        case KmpUnit::kW: return "kW";
        case KmpUnit::MW: return "MW";
        case KmpUnit::C: return "C";
        case KmpUnit::K: return "K";
        case KmpUnit::L: return "l";
        case KmpUnit::M3: return "m3";
        case KmpUnit::L_PER_H: return "l/h";
        case KmpUnit::M3_PER_H: return "m3/h";
        case KmpUnit::M3XC: return "m3*C";
        case KmpUnit::TON: return "ton";
        case KmpUnit::TON_PER_H: return "ton/h";
        case KmpUnit::H: return "h";
        case KmpUnit::CLOCK: return "clock";
        case KmpUnit::DATE1: return "date1";
        case KmpUnit::DATE3: return "date3";
        case KmpUnit::NUMBER: return "number";
        case KmpUnit::BAR: return "bar";
        default: return "UNKNOWN";
    }
}

} // namespace

#endif //_KMP_UNITS_HPP_
