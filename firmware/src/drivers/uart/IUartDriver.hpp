#ifndef _I_UART_DRIVER_HPP_
#define _I_UART_DRIVER_HPP_

#include "UartConfig.hpp"

#include <cstdint>
#include <cstddef>
#include <vector>
#include <functional>

namespace drivers::uart {

class IUartDriver {
public:
    virtual ~IUartDriver() = default;

    // Initialize UART with configuration.
    virtual bool init(const UartConfig &config) = 0;

    // Write raw bytes to UART. Returns number of bytes written, or -1 on error.
    virtual int write(const std::vector<uint8_t>& data) = 0;

    // Read up to len bytes into buffer with timeout in ms. Returns number of bytes read, or -1 on error.
    virtual int read(uint8_t* buffer, size_t len, uint32_t timeoutMs) = 0;

    // Set Rx callback that will be invoked when data arrives.
    virtual void setRxCallback(std::function<void(const std::vector<uint8_t>&)> cb) = 0;

    // Poll for incoming data and invoke the registered callback.
    virtual void process() = 0;
};

}

#endif //_I_UART_DRIVER_HPP_
