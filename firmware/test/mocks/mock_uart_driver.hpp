#ifndef _MOCK_UART_DRIVER_HPP_
#define _MOCK_UART_DRIVER_HPP_

#include "src/drivers/uart/IUartDriver.hpp"
#include <functional>
#include <vector>
#include <cstdint>
#include <cstddef>

struct MockUartDriver : public drivers::uart::IUartDriver {
    bool init(const drivers::uart::UartConfig &config) override { return true; }

    int write(const std::vector<uint8_t>& data) override {
        lastWritten = data;
        return writeResult >= 0 ? writeResult : static_cast<int>(data.size());
    }

    int read(uint8_t* buffer, size_t len, uint32_t timeoutMs) override { return 0; }

    void setRxCallback(std::function<void(const std::vector<uint8_t>&)> cb) override {
        rxCallback_ = cb;
    }

    void process() override {}

    void receiveBytes(const std::vector<uint8_t>& data) {
        if (rxCallback_) rxCallback_(data);
    }

    std::vector<uint8_t> lastWritten;
    int writeResult = 0;

private:
    std::function<void(const std::vector<uint8_t>&)> rxCallback_;
};

#endif //_MOCK_UART_DRIVER_HPP_
