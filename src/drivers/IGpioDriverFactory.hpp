#ifndef _I_GPIO_DRIVER_FACTORY_HPP_
#define _I_GPIO_DRIVER_FACTORY_HPP_

#include "gpio/IGpioDriver.hpp"

#include <memory>

namespace drivers {

class IGpioDriverFactory
{
public:
    ~IGpioDriverFactory() = default;

    virtual std::shared_ptr<gpio::IGpioDriver> getGpioDriver() = 0;
};

}

#endif //_I_GPIO_DRIVER_FACTORY_HPP_




