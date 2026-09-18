#ifndef _UART_CONFIG_HPP_
#define _UART_CONFIG_HPP_

#include <cstdint>

namespace drivers::uart {

struct UartConfig
{
    uint32_t baud;
    uint8_t dataBits;
    uint8_t parity;
    uint8_t stopBits;
    int8_t txPin = -1;  // -1 = use default pins for the selected UART
    int8_t rxPin = -1;  // -1 = use default pins for the selected UART
    bool invertTx = false;
    bool invertRx = false;
};

}

#endif //_UART_CONFIG_HPP_
