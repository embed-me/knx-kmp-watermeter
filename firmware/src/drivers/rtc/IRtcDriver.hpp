#ifndef _I_RTC_DRIVER_HPP_
#define _I_RTC_DRIVER_HPP_

#include <cstdint>

namespace drivers::rtc {

struct DateTime {
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
};

class IRtcDriver {
public:
    virtual ~IRtcDriver() = default;
    virtual void setDateTime(const DateTime& dt) = 0;
    virtual DateTime getDateTime() = 0;
};

}

#endif // _I_RTC_DRIVER_HPP_
