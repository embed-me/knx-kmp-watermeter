#ifndef _I_GPIO_DRIVER_HPP_
#define _I_GPIO_DRIVER_HPP_

#include "GpioConfig.hpp"

#include <functional>

namespace drivers::gpio {

typedef void (*IsrFunctionPtr)();

class IGpioDriver
{
public:
    virtual ~IGpioDriver() = default;

    virtual void setConfig(const GpioConfig &gpio) = 0;
    virtual bool readValue(const GpioConfig &gpio) = 0;
    virtual bool writeValue(const GpioConfig &gpio, bool value) = 0;
    virtual bool setupInterruptHandler(const GpioConfig &gpio, IsrFunctionPtr handler) = 0;
};

}

#endif //_I_GPIO_DRIVER_HPP_