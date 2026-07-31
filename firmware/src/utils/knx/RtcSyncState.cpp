#include "RtcSyncState.hpp"

namespace utils::knx {

bool RtcSyncState::updateTime(const KnxTime& t)
{
    dt_.hour = t.hour;
    dt_.minute = t.minute;
    dt_.second = t.second;
    haveTime_ = true;
    return ready();
}

bool RtcSyncState::updateDate(const KnxDate& d)
{
    dt_.year = d.year;
    dt_.month = d.month;
    dt_.day = d.day;
    haveDate_ = true;
    return ready();
}

bool RtcSyncState::ready() const
{
    return haveTime_ && haveDate_;
}

const drivers::rtc::DateTime& RtcSyncState::dateTime() const
{
    return dt_;
}

}
