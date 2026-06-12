#ifndef _MELODY_CONFIG_HPP_
#define _MELODY_CONFIG_HPP_

#include <stdint.h>
#include <memory>

#include "IMode.hpp"
#include "Melody.hpp"

namespace drivers::knx {

struct MelodyConfig
{
    uint8_t priority;
    std::shared_ptr<IMode> mode;
    Melody melody;
};

}

#endif //_MELODY_CONFIG_HPP_