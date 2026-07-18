#include "ArduinoRtcDriver.hpp"

#include <sys/time.h>
#include <ctime>

using namespace drivers::rtc;

void ArduinoRtcDriver::setDateTime(const DateTime& dt)
{
    struct tm tm = {};
    tm.tm_year = dt.year - 1900;
    tm.tm_mon  = dt.month - 1;
    tm.tm_mday = dt.day;
    tm.tm_hour = dt.hour;
    tm.tm_min  = dt.minute;
    tm.tm_sec  = dt.second;
    tm.tm_isdst = 0;

    time_t t = mktime(&tm);
    struct timeval tv = {};
    tv.tv_sec = t;
    tv.tv_usec = 0;
    settimeofday(&tv, nullptr);
}

DateTime ArduinoRtcDriver::getDateTime()
{
    time_t now = time(nullptr);
    struct tm* tm = localtime(&now);

    DateTime dt = {};
    dt.year   = tm->tm_year + 1900;
    dt.month  = static_cast<uint8_t>(tm->tm_mon + 1);
    dt.day    = static_cast<uint8_t>(tm->tm_mday);
    dt.hour   = static_cast<uint8_t>(tm->tm_hour);
    dt.minute = static_cast<uint8_t>(tm->tm_min);
    dt.second = static_cast<uint8_t>(tm->tm_sec);
    return dt;
}
