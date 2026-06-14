#ifndef _I_APPLICATION_HPP_
#define _I_APPLICATION_HPP_

#include "../drivers/uart/IUartDriver.hpp"
#include "../drivers/knx/data/KnxConfig.hpp"

#include <memory>

namespace application {

class IApplication {
public:
    virtual ~IApplication() = default;

    virtual void init(
        std::shared_ptr<drivers::uart::IUartDriver> uart,
        const drivers::uart::UartConfig& uartConfig,
        drivers::knx::KnxConfig& knxConfig
    ) = 0;
    virtual void process() = 0;
};

}

#endif // _I_APPLICATION_HPP_
