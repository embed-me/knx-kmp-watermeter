#include "ArduinoUartDriver.hpp"
#include "src/drivers/logger/Logger.hpp"
#include <Arduino.h>
#include <HardwareSerial.h>
#include <hardware/gpio.h>

using namespace drivers::logger;

namespace drivers::uart {

ArduinoUartDriver::ArduinoUartDriver()
{
}

bool ArduinoUartDriver::init(const UartConfig &config) 
{
    if (config.dataBits < 5 || config.dataBits > 8) {
        logError("ArduinoUartDriver: invalid dataBits %d", config.dataBits);
        return false;
    }

    if (config.stopBits < 1 || config.stopBits > 2) {
        logError("ArduinoUartDriver: invalid stopBits %d", config.stopBits);
        return false;
    }

    if (config.parity > 2) {
        logError("ArduinoUartDriver: invalid parity %d", config.parity);
        return false;
    }

    // Use Arduino Serial2 (UART1) for KMP communication.
    // Serial1 (UART0) is already used by the KNX stack (KNX_SERIAL).
    if (config.txPin >= 0) {
        Serial2.setTX(config.txPin);
    }
    if (config.rxPin >= 0) {
        Serial2.setRX(config.rxPin);
    }

    int serialConfig = SERIAL_8N1;
    if (config.dataBits == 5) {
        serialConfig = (config.parity == 0) ? SERIAL_5N1 : (config.parity == 1) ? SERIAL_5O1 : SERIAL_5E1;
    } else if (config.dataBits == 6) {
        serialConfig = (config.parity == 0) ? SERIAL_6N1 : (config.parity == 1) ? SERIAL_6O1 : SERIAL_6E1;
    } else if (config.dataBits == 7) {
        serialConfig = (config.parity == 0) ? SERIAL_7N1 : (config.parity == 1) ? SERIAL_7O1 : SERIAL_7E1;
    } else if (config.dataBits == 8) {
        if (config.stopBits == 2) {
            serialConfig = (config.parity == 0) ? SERIAL_8N2 : (config.parity == 1) ? SERIAL_8O2 : SERIAL_8E2;
        } else {
            serialConfig = (config.parity == 0) ? SERIAL_8N1 : (config.parity == 1) ? SERIAL_8O1 : SERIAL_8E1;
        }
    }

    Serial2.begin(config.baud, serialConfig);
    if (config.txPin >= 0) {
        gpio_set_outover(config.txPin,
                         config.invertTx ? GPIO_OVERRIDE_INVERT : GPIO_OVERRIDE_NORMAL);
    }
    if (config.rxPin >= 0) {
        gpio_set_inover(config.rxPin,
                        config.invertRx ? GPIO_OVERRIDE_INVERT : GPIO_OVERRIDE_NORMAL);
    }
    logInfo("ArduinoUartDriver: initialized at %d baud (%d data bits, %d stop bits, tx=%d rx=%d)", config.baud, config.dataBits, config.stopBits, config.txPin, config.rxPin);
    return true;
}

int ArduinoUartDriver::write(const std::vector<uint8_t>& data) 
{
    if (data.empty()) {
        return 0;
    }

    for (const auto& b : data) {
        logTrace("transmitting byte 0x%02X", b);
    }
    size_t written = Serial2.write(data.data(), data.size());

    if (written < data.size()) {
        logWarning("ArduinoUartDriver: write incomplete (%u of %u bytes)", (uint16_t)written, (uint16_t)data.size());
    }

    return static_cast<int>(written);
}

int ArduinoUartDriver::read(uint8_t* buffer, size_t len, uint32_t timeoutMs) 
{
    if (!buffer || len == 0) {
        return -1;
    }

    uint32_t start = millis();
    uint16_t bytesRead = 0;

    while (bytesRead < len && (millis() - start) < timeoutMs) {
        if (Serial2.available()) {
            int byte = Serial2.read();
            if (byte >= 0) {
                buffer[bytesRead++] = static_cast<uint8_t>(byte);
            }
        }
    }

    return static_cast<int>(bytesRead);
}

void ArduinoUartDriver::setRxCallback(std::function<void(const std::vector<uint8_t>&)> cb)
{
    rx_callback_ = cb;
}

void ArduinoUartDriver::process()
{
    if (!rx_callback_) {
        return;
    }
    std::vector<uint8_t> chunk;
    while (Serial2.available()) {
        int b = Serial2.read();
        if (b >= 0) {
            chunk.push_back(static_cast<uint8_t>(b));
            logTrace("received byte 0x%02X", b);
        }
    }
    if (!chunk.empty()) {
        rx_callback_(chunk);
    }
}

}
