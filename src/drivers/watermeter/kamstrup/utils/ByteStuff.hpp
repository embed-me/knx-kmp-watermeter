#ifndef _BYTE_STUFF_HPP_
#define _BYTE_STUFF_HPP_

#include <vector>
#include <cstddef>
#include <cstdint>

namespace drivers::watermeter::kamstrup::transport::utils {

std::vector<uint8_t> stuff_bytes(const std::vector<uint8_t>& data);
std::vector<uint8_t> unstuff_bytes(const std::vector<uint8_t>& data);

}

#endif //_BYTE_STUFF_HPP_
