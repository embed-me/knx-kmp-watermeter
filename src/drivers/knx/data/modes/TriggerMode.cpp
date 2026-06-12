#include "TriggerMode.hpp"

using namespace drivers::knx;

TriggerMode::TriggerMode(uint16_t ko) :
    ko_number(ko)
{
}

enum ModeType TriggerMode::getType() const
{
    return ModeType::MODE_TRIGGER;
}
