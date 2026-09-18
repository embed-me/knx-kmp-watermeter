#ifndef _ARDUINO_RTC_DRIVER_HPP_
#define _ARDUINO_RTC_DRIVER_HPP_

#include "../IRtcDriver.hpp"

namespace drivers::rtc {

class ArduinoRtcDriver : public IRtcDriver {
public:
    ArduinoRtcDriver() = default;
    ~ArduinoRtcDriver() override = default;

    void setDateTime(const DateTime& dt) override;
    DateTime getDateTime() override;
};

}

#endif // _ARDUINO_RTC_DRIVER_HPP_
