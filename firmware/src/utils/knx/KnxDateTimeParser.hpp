#ifndef _KNX_DATETIME_PARSER_HPP_
#define _KNX_DATETIME_PARSER_HPP_

#include <cstdint>
#include <cstddef>

namespace utils::knx {

struct KnxTime {
    bool valid = false;
    uint8_t hour = 0;
    uint8_t minute = 0;
    uint8_t second = 0;
};

struct KnxDate {
    bool valid = false;
    uint16_t year = 0;
    uint8_t month = 0;
    uint8_t day = 0;
};

KnxTime parseKnxTime(const uint8_t* data, size_t size);
KnxDate parseKnxDate(const uint8_t* data, size_t size);

}

#endif // _KNX_DATETIME_PARSER_HPP_
