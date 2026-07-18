#ifndef _CRON_MATCHER_HPP_
#define _CRON_MATCHER_HPP_

#include "src/drivers/rtc/IRtcDriver.hpp"

#include <string>
#include <vector>
#include <cstdint>

namespace utils::cron {

struct CronField {
    enum Type { STAR, STAR_SLASH, VALUE, RANGE, LIST } type = STAR;
    int value = 0;
    int step = 1;
    int rangeStart = 0;
    int rangeEnd = 0;
    std::vector<int> values;
};

class CronMatcher {
public:
    explicit CronMatcher(const std::string& expression);
    bool matches(const drivers::rtc::DateTime& dt) const;

private:
    CronField parseField(const std::string& token, int minVal, int maxVal) const;
    bool fieldMatches(const CronField& field, int value) const;

    CronField minute_;
    CronField hour_;
    CronField dayOfMonth_;
    CronField month_;
    CronField dayOfWeek_;
    bool valid_ = false;
};

}

#endif // _CRON_MATCHER_HPP_
