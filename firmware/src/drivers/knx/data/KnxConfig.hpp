#ifndef _KNX_CONFIG_HPP_
#define _KNX_CONFIG_HPP_

#include <knx.h>
#include "knxprod.h"

#include "src/drivers/logger/Logger.hpp"

#include <list>
#include <unordered_map>
#include <array>
#include <string>
#include <cstring>


using namespace drivers::logger;

namespace drivers::knx {


struct KnxApplicationVersion {
    uint8_t major;
    uint8_t minor;
};


struct KnxWatermeterConfig {
    uint8_t destinationAddress = 0x3F;
    uint32_t keepAliveIntervalSec = 10;
};

struct WatermeterRegisterConfig {
    uint16_t registerId;
    const char* name;
    bool enabled;
    GroupObject* groupObject;
    Dpt dpt;
};

struct KnxCommunicationObject {
    GroupObject* groupObject = nullptr;
    Dpt dpt = Dpt();
};

class KnxConfig
{
public:
    explicit KnxConfig(KnxFacade<RP2040ArduinoPlatform, Bau07B0> &knx) : knx(knx) {};
    ~KnxConfig() = default;

    KnxWatermeterConfig getWatermeterConfig()
    {
        KnxWatermeterConfig config;
        config.keepAliveIntervalSec = ParamAPP_Keep_Alive_Interval;
        return config;
    }

    std::array<WatermeterRegisterConfig, 4> getWatermeterRegisterConfigs()
    {
        return {{
            {0x0044, "Volume",       ParamAPP_Volume,       &KoAPP_Volume,       DPT_Value_Volume},
            {0x004A, "Flow",         ParamAPP_Flow,         &KoAPP_Flow,         DPT_Value_Volume_Flow},
            {0x0124, "Temperature",  ParamAPP_Temperature,  &KoAPP_Temperature,  DPT_Value_Temp},
            {0x0246, "Battery Life", ParamAPP_Battery_Life, &KoAPP_Battery_Life, DPT_Value_2_Ucount},
        }};
    }

    KnxCommunicationObject getKeepAliveLinkedState()
    {
        return {&KoAPP_Linked, DPT_State};
    }

    std::string getPollingCronExpression()
    {
        constexpr size_t CRON_SIZE = 16;
        char buf[CRON_SIZE + 1] = {};
        for (size_t i = 0; i < CRON_SIZE; i++) {
            buf[i] = static_cast<char>(knx.paramData(APP_Polling_Interval_(cron_schedule_expression))[i]);
        }
        buf[CRON_SIZE] = '\0';
        return std::string(buf);
    }

    GroupObject& getDateTimeGroupObject()
    {
        return KoAPP_DateTime;
    }

    struct KnxApplicationVersion getApplicationVersion()
    {
        struct KnxApplicationVersion appVersion = {
            .major = (MAIN_ApplicationVersion >> 4) & 0x0f,
            .minor = (MAIN_ApplicationVersion >> 0) & 0x0f
        };
        return appVersion;
    }

private:
    KnxFacade<RP2040ArduinoPlatform, Bau07B0> &knx;
};

}

#endif //_KNX_CONFIG_HPP_
