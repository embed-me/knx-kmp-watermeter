#ifndef _I_KNX_DRIVER_HPP_
#define _I_KNX_DRIVER_HPP_

#include "data/KnxConfig.hpp"
#include "knx.h"

namespace drivers::knx {

class IKnxDriver
{
public:
    virtual ~IKnxDriver() = default;

    virtual bool init() = 0;
    virtual std::string getApplicationVersion() = 0;
    virtual KnxConfig &getKnxConfig() = 0;
    virtual void setProgmodeChangeCallback(std::function<void(bool)> callback) = 0;
    virtual void register_cb(int ko_number, std::function<void(GroupObject&)>) = 0;
    virtual void toggleProgMode() = 0;
    virtual void loop() = 0;
};

}

#endif //_I_KNX_DRIVER_HPP_