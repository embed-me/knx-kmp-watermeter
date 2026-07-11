#ifndef _I_WATERMETER_WAKEUP_DRIVER_HPP_
#define _I_WATERMETER_WAKEUP_DRIVER_HPP_

namespace drivers::watermeter::wakeup {

class IWatermeterWakeupDriver {
public:
    virtual ~IWatermeterWakeupDriver() = default;

    virtual void wakeup() = 0;
    virtual void sleep() = 0;
    virtual bool isAwake() const = 0;
};

}

#endif //_I_WATERMETER_WAKEUP_DRIVER_HPP_
