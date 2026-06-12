#ifndef _I_MODE_HPP_
#define _I_MODE_HPP_

#include <string>
#include <stdint.h>

namespace drivers::knx {

enum ModeType {
    MODE_TRIGGER = 0,
    MODE_SWITCH = 1,
    MODE_VENTING_MONITOR = 2
};

class IMode
{
public:
    virtual ~IMode() = default;

    virtual enum ModeType getType() const = 0;
};

}

#endif //_I_MODE_HPP_