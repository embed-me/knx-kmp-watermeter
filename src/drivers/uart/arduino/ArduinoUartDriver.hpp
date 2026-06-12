#ifndef _ARDUINO_UART_DRIVER_HPP_
#define _ARDUINO_UART_DRIVER_HPP_

#include "../IUartDriver.hpp"
#include <memory>

namespace drivers::uart {

class ArduinoUartDriver : public IUartDriver {
public:
    ArduinoUartDriver();
    ~ArduinoUartDriver() = default;

    virtual bool init(const UartConfig &config) override;
    virtual int write(const std::vector<uint8_t>& data) override;
    virtual int read(uint8_t* buffer, size_t len, uint32_t timeoutMs) override;
    virtual void setRxCallback(std::function<void(const std::vector<uint8_t>&)> cb) override;
    virtual void process() override;

private:
    std::function<void(const std::vector<uint8_t>&)> rx_callback_;
};

}

#endif //_ARDUINO_UART_DRIVER_HPP_
