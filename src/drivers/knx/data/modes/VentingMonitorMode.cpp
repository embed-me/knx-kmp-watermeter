#include "VentingMonitorMode.hpp"

using namespace drivers::knx;

VentingMonitorMode::VentingMonitorMode(
        uint16_t ko_venting,
        uint16_t ko_temp, 
        uint8_t lower_temp_limit_c, 
        uint8_t upper_temp_limit_c,
        uint8_t venting_duration_limit_min) :
    ko_venting(ko_venting),
    ko_temp(ko_temp),
    lower_temp_limit_c(lower_temp_limit_c),
    upper_temp_limit_c(upper_temp_limit_c),
    venting_duration_limit_min(venting_duration_limit_min)
{
}

enum ModeType VentingMonitorMode::getType() const
{
    return ModeType::MODE_VENTING_MONITOR;
}
