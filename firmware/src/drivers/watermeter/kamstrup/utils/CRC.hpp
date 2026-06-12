#ifndef _KMP_CRC_HPP_
#define _KMP_CRC_HPP_

#include <cstdint>
#include <cstddef>
#include <vector>

namespace drivers::watermeter::kamstrup::transport::utils {

uint16_t crc16_ccitt(const uint8_t* data, size_t len);
uint16_t crc16_ccitt(const std::vector<uint8_t>& data);

}

#endif //_KMP_CRC_HPP_
