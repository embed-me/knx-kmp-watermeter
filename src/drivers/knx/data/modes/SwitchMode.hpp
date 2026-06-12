#ifndef _SWITCH_MODE_HPP_
#define _SWITCH_MODE_HPP_

#include "../IMode.hpp"

namespace drivers::knx {

class SwitchMode : public IMode
{
public:
    explicit SwitchMode(uint16_t ko);
    ~SwitchMode() = default;

    virtual enum ModeType getType() const override;
    uint16_t ko_number = 0;
};

}

#endif //_SWITCH_MODE_HPP_