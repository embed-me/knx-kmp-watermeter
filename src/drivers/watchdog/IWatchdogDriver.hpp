#ifndef _I_WATCHDOG_DRIVER_HPP_
#define _I_WATCHDOG_DRIVER_HPP_

#include <stdint.h>

namespace drivers::watchdog {

class IWatchdogDriver {
public:
    virtual ~IWatchdogDriver() = default;

    virtual void feed() = 0;
    virtual void enable(uint32_t timeout_ms) = 0;
};

}

#endif //_I_WATCHDOG_DRIVER_HPP_ 