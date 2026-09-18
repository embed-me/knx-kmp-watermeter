#include "CronMatcher.hpp"
#include <sstream>
#include <cctype>
#include <algorithm>
#include <ctime>

using namespace utils::cron;

CronMatcher::CronMatcher(const std::string& expression)
{
    std::istringstream iss(expression);
    std::string tokens[5];
    for (int i = 0; i < 5; i++) {
        if (!(iss >> tokens[i])) {
            return;
        }
    }

    minute_     = parseField(tokens[0], 0, 59);
    hour_       = parseField(tokens[1], 0, 23);
    dayOfMonth_ = parseField(tokens[2], 1, 31);
    month_      = parseField(tokens[3], 1, 12);
    dayOfWeek_  = parseField(tokens[4], 0, 7);

    valid_ = true;
}

bool CronMatcher::matches(const drivers::rtc::DateTime& dt) const
{
    if (!valid_) return false;

    int dw = 0;
    struct tm tm = {};
    tm.tm_year = dt.year - 1900;
    tm.tm_mon  = dt.month - 1;
    tm.tm_mday = dt.day;
    tm.tm_hour = dt.hour;
    tm.tm_min  = dt.minute;
    tm.tm_isdst = 0;
    if (mktime(&tm) != static_cast<time_t>(-1)) {
        dw = tm.tm_wday;
        if (dw == 0) dw = 7;
    }

    return fieldMatches(minute_, dt.minute) &&
           fieldMatches(hour_, dt.hour) &&
           fieldMatches(dayOfMonth_, dt.day) &&
           fieldMatches(month_, dt.month) &&
           fieldMatches(dayOfWeek_, dw);
}

CronField CronMatcher::parseField(const std::string& token, int minVal, int maxVal) const
{
    CronField field;

    if (token == "*") {
        field.type = CronField::STAR;
        return field;
    }

    if (token.size() > 2 && token[0] == '*' && token[1] == '/') {
        field.type = CronField::STAR_SLASH;
        field.step = std::stoi(token.substr(2));
        return field;
    }

    if (token.find(',') != std::string::npos) {
        field.type = CronField::LIST;
        std::istringstream ss(token);
        std::string part;
        while (std::getline(ss, part, ',')) {
            field.values.push_back(std::stoi(part));
        }
        return field;
    }

    if (token.find('-') != std::string::npos) {
        field.type = CronField::RANGE;
        size_t dash = token.find('-');
        field.rangeStart = std::stoi(token.substr(0, dash));
        field.rangeEnd = std::stoi(token.substr(dash + 1));
        return field;
    }

    field.type = CronField::VALUE;
    field.value = std::stoi(token);
    return field;
}

bool CronMatcher::fieldMatches(const CronField& field, int value) const
{
    switch (field.type) {
        case CronField::STAR:
            return true;
        case CronField::STAR_SLASH:
            if (field.step == 0) return false;
            return (value % field.step) == 0;
        case CronField::VALUE:
            return value == field.value;
        case CronField::RANGE:
            return value >= field.rangeStart && value <= field.rangeEnd;
        case CronField::LIST:
            return std::find(field.values.begin(), field.values.end(), value) != field.values.end();
    }
    return false;
}
