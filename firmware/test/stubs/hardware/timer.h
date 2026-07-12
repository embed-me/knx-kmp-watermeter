#pragma once
#include "pico/time.h"

static alarm_id_t stub_next_alarm_id = 1;

static inline alarm_id_t add_alarm_in_us(uint32_t delay_us, int64_t (*callback)(alarm_id_t, void*), void* user_data, bool fire_if_past) {
    (void)delay_us;
    (void)callback;
    (void)user_data;
    (void)fire_if_past;
    return stub_next_alarm_id++;
}

static inline bool cancel_alarm(alarm_id_t alarm_id) {
    (void)alarm_id;
    return true;
}
