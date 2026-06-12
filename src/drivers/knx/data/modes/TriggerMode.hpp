#ifndef _TRIGGER_MODE_HPP_
#define _TRIGGER_MODE_HPP_

#include "../IMode.hpp"

namespace drivers::knx {

class TriggerMode : public IMode
{
public:
    explicit TriggerMode(uint16_t ko);
    ~TriggerMode() = default;

    virtual enum ModeType getType() const override;
    uint16_t ko_number = 0;
};

}

#endif //_TRIGGER_MODE_HPP_