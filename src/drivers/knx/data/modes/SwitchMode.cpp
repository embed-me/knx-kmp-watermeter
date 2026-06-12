#include "SwitchMode.hpp"

using namespace drivers::knx;

SwitchMode::SwitchMode(uint16_t ko) :
    ko_number(ko)
{
}

enum ModeType SwitchMode::getType() const
{
    return ModeType::MODE_SWITCH;
}
