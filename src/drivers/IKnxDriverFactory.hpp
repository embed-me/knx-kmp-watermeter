#ifndef _I_KNX_DRIVER_FACTORY_HPP_
#define _I_KNX_DRIVER_FACTORY_HPP_

#include "knx/IKnxDriver.hpp"

#include <memory>

namespace drivers {

class IKnxDriverFactory
{
public:
    ~IKnxDriverFactory() = default;

    virtual std::shared_ptr<knx::IKnxDriver> getKnxDriver() = 0;
};

}

#endif //_I_KNX_DRIVER_FACTORY_HPP_




