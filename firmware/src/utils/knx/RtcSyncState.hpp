#ifndef _RTC_SYNC_STATE_HPP_
#define _RTC_SYNC_STATE_HPP_

#include "KnxDateTimeParser.hpp"
#include "../../drivers/rtc/IRtcDriver.hpp"

namespace utils::knx {

// Merges separately received KNX time and date telegrams into a single
// drivers::rtc::DateTime. The RTC may only be written once BOTH components
// have been seen; every subsequent valid telegram re-syncs the RTC with the
// merged value, keeping the last-known component of the other kind.
// Structs with valid==false are ignored entirely: nothing is stored and the
// seen-flag is not set, so a corrupt telegram can never clobber good data or
// advance the state machine.
class RtcSyncState {
public:
    // Stores hour/minute/second from t and returns true once both time and
    // date have been seen. If t.valid is false nothing is stored and the
    // seen-flag is not set; the return value still reports whether both were
    // already seen.
    bool updateTime(const KnxTime& t);

    // Stores year/month/day from d and returns true once both time and date
    // have been seen. If d.valid is false nothing is stored and the seen-flag
    // is not set; the return value still reports whether both were already
    // seen.
    bool updateDate(const KnxDate& d);

    // The merged date/time accumulated so far.
    const drivers::rtc::DateTime& dateTime() const;

private:
    // True once both a time and a date telegram have been seen.
    bool ready() const;

    drivers::rtc::DateTime dt_ = {};
    bool haveTime_ = false;
    bool haveDate_ = false;
};

}

#endif // _RTC_SYNC_STATE_HPP_
