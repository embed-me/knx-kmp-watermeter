#ifndef _GPIO_CONFIG_HPP_
#define _GPIO_CONFIG_HPP_

#include <stdint.h>

namespace drivers::gpio {

enum GPIO_DIRECTION {
    UNUSED = 0,
    DIRECTION_INPUT = 1,
    DIRECTION_OUTPUT = 2
};

enum GPIO_SENSITIVITY {
    ACTIVE_LOW = 0,
    ACTIVE_HIGH = 1,
    INTERRUPT_CHANGE = 2
};

enum GPIO_PULL {
    PULL_NONE = 0,
    PULL_LOW = 1,
    PULL_HIGH = 2
};

struct GpioConfig
{
    const uint32_t pin;
    const GPIO_SENSITIVITY sensitivity;
    const GPIO_DIRECTION direction;
    const GPIO_PULL pull;
};

}

#endif //_GPIO_CONFIG_HPP_