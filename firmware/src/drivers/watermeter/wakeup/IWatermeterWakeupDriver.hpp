#ifndef _I_WATERMETER_WAKEUP_DRIVER_HPP_
#define _I_WATERMETER_WAKEUP_DRIVER_HPP_

#include <functional>
#include <cstdint>

namespace drivers::watermeter::wakeup {

class IWatermeterWakeupDriver {
public:
    virtual ~IWatermeterWakeupDriver() = default;

    virtual void wakeup(std::function<void()> onReady) = 0;
    virtual void sleep() = 0;
};

}

#endif //_I_WATERMETER_WAKEUP_DRIVER_HPP_
