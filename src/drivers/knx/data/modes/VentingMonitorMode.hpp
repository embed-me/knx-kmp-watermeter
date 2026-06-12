#ifndef _VENTING_MONITOR_MODE_HPP_
#define _VENTING_MONITOR_MODE_HPP_

#include "../IMode.hpp"

namespace drivers::knx {

class VentingMonitorMode : public IMode
{
public:
    VentingMonitorMode(
        uint16_t ko_venting, 
        uint16_t ko_temp, 
        uint8_t lower_temp_limit_c, 
        uint8_t upper_temp_limit_c,
        uint8_t venting_duration_limit_min);
    ~VentingMonitorMode() = default;

    virtual enum ModeType getType() const override;
    uint16_t ko_venting = 0;
    uint16_t ko_temp = 0;
    uint8_t lower_temp_limit_c = 0;
    uint8_t upper_temp_limit_c = 0;
    uint8_t venting_duration_limit_min = 0;
};

}

#endif //_VENTING_MONITOR_MODE_HPP_