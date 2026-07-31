#include "KnxDateTimeParser.hpp"

namespace utils::knx {

constexpr uint8_t kDaysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

constexpr bool isLeapYear(uint16_t year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

KnxTime parseKnxTime(const uint8_t* data, size_t size)
{
    KnxTime result;

    if (data == nullptr || size != 3) {
        return result;
    }

    uint8_t hour = data[0] & 0x1F;
    uint8_t minute = data[1] & 0x3F;
    uint8_t second = data[2] & 0x3F;

    if (hour > 23 || minute > 59 || second > 59) {
        return result;
    }

    result.valid = true;
    result.hour = hour;
    result.minute = minute;
    result.second = second;
    return result;
}

KnxDate parseKnxDate(const uint8_t* data, size_t size)
{
    KnxDate result;

    if (data == nullptr || size != 3) {
        return result;
    }

    uint8_t day = data[0] & 0x1F;
    uint8_t month = data[1] & 0x0F;
    uint8_t yearField = data[2] & 0x7F;

    if (day < 1 || day > 31 || month < 1 || month > 12 || yearField > 99) {
        return result;
    }

    uint16_t year = yearField >= 90 ? 1900 + yearField : 2000 + yearField;

    uint8_t maxDay = kDaysInMonth[month - 1];
    if (month == 2 && isLeapYear(year)) {
        maxDay = 29;
    }
    if (day > maxDay) {
        return result;
    }

    result.valid = true;
    result.day = day;
    result.month = month;
    result.year = year;
    return result;
}

}
