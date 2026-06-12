#ifndef _MELODY_HPP_
#define _MELODY_HPP_

#include <string>
#include <stdint.h>

namespace drivers::knx {

struct Melody
{
    const char *rtttl;
    uint8_t pause_sec;
};

}

#endif //_MELODY_HPP_