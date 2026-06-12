#ifndef _KNX_CONFIG_HPP_
#define _KNX_CONFIG_HPP_

#include <knx.h>
#include "knxprod.h"
#include "MelodyConfig.hpp"
#include "modes/TriggerMode.hpp"
#include "modes/SwitchMode.hpp"
#include "modes/VentingMonitorMode.hpp"

#include "src/drivers/logger/Logger.hpp"

#include <list>


using namespace drivers::logger;

namespace drivers::knx {


struct ApplicationVersion {
    uint8_t major;
    uint8_t minor;
};


class KnxConfig
{
public:
    explicit KnxConfig(KnxFacade<RP2040ArduinoPlatform, Bau07B0> &knx) : knx(knx) {};
    ~KnxConfig() = default;

    std::list<MelodyConfig> getMelodyConfigs()
    {
        MelodyConfig config;
        std::list<MelodyConfig> configs;
        Melody melody;

        switch(ParamAPP_Number_Melodies) {
            case 8: {

                melody.rtttl = reinterpret_cast<const char *>(ParamAPP_Melody_8);
                melody.pause_sec = ParamAPP_Melody_8_Melody_Pause;
                config.melody = melody;

                config.priority = 8; //TODO ADD PRIO FROM KNX

                switch(ParamAPP_Melody_8___Mode)
                {
                    case ModeType::MODE_TRIGGER:
                        config.mode = std::make_shared<TriggerMode>(APP_KoTrigger_Melody_8);
                        break;
                    case ModeType::MODE_SWITCH:
                        config.mode = std::make_shared<SwitchMode>(APP_KoSWITCH_Melody_8);
                        break;
                    case ModeType::MODE_VENTING_MONITOR:
                    default:
                        config.mode = std::make_shared<VentingMonitorMode>(
                            APP_KoSWITCH_Melody_8,
                            APP_KoTemperature_Melody_8,
                            ParamAPP_Melody_8___Lower_Temp_Exclusion_Limit,
                            ParamAPP_Melody_8___Upper_Temp_Exclusion_Limit,
                            ParamAPP_Melody_8___Duration);
                        break;
                }

                configs.push_back(config);
                [[fallthrough]];
            }
            case 7: {

                melody.rtttl = reinterpret_cast<const char *>(ParamAPP_Melody_7);
                melody.pause_sec = ParamAPP_Melody_7_Melody_Pause;
                config.melody = melody;

                config.priority = 7; //TODO ADD PRIO FROM KNX

                switch(ParamAPP_Melody_7___Mode)
                {
                    case ModeType::MODE_TRIGGER:
                        config.mode = std::make_shared<TriggerMode>(APP_KoTrigger_Melody_7);
                        break;
                    case ModeType::MODE_SWITCH:
                        config.mode = std::make_shared<SwitchMode>(APP_KoSWITCH_Melody_7);
                        break;
                    case ModeType::MODE_VENTING_MONITOR:
                    default:
                        config.mode = std::make_shared<VentingMonitorMode>(
                            APP_KoSWITCH_Melody_7,
                            APP_KoTemperature_Melody_7,
                            ParamAPP_Melody_7___Lower_Temp_Exclusion_Limit,
                            ParamAPP_Melody_7___Upper_Temp_Exclusion_Limit,
                            ParamAPP_Melody_7___Duration);
                        break;
                }

                configs.push_back(config);
                [[fallthrough]];
            }
            case 6: {

                melody.rtttl = reinterpret_cast<const char *>(ParamAPP_Melody_6);
                melody.pause_sec = ParamAPP_Melody_6_Melody_Pause;
                config.melody = melody;

                config.priority = 6; //TODO ADD PRIO FROM KNX

                switch(ParamAPP_Melody_6___Mode)
                {
                    case ModeType::MODE_TRIGGER:
                        config.mode = std::make_shared<TriggerMode>(APP_KoTrigger_Melody_6);
                        break;
                    case ModeType::MODE_SWITCH:
                        config.mode = std::make_shared<SwitchMode>(APP_KoSWITCH_Melody_6);
                        break;
                    case ModeType::MODE_VENTING_MONITOR:
                    default:
                        config.mode = std::make_shared<VentingMonitorMode>(
                            APP_KoSWITCH_Melody_6,
                            APP_KoTemperature_Melody_6,
                            ParamAPP_Melody_6___Lower_Temp_Exclusion_Limit,
                            ParamAPP_Melody_6___Upper_Temp_Exclusion_Limit,
                            ParamAPP_Melody_6___Duration);
                        break;
                }

                configs.push_back(config);
                [[fallthrough]];
            }
            case 5: {

                melody.rtttl = reinterpret_cast<const char *>(ParamAPP_Melody_5);
                melody.pause_sec = ParamAPP_Melody_5_Melody_Pause;
                config.melody = melody;

                config.priority = 5; //TODO ADD PRIO FROM KNX

                switch(ParamAPP_Melody_5___Mode)
                {
                    case ModeType::MODE_TRIGGER:
                        config.mode = std::make_shared<TriggerMode>(APP_KoTrigger_Melody_5);
                        break;
                    case ModeType::MODE_SWITCH:
                        config.mode = std::make_shared<SwitchMode>(APP_KoSWITCH_Melody_5);
                        break;
                    case ModeType::MODE_VENTING_MONITOR:
                    default:
                        config.mode = std::make_shared<VentingMonitorMode>(
                            APP_KoSWITCH_Melody_5,
                            APP_KoTemperature_Melody_5,
                            ParamAPP_Melody_5___Lower_Temp_Exclusion_Limit,
                            ParamAPP_Melody_5___Upper_Temp_Exclusion_Limit,
                            ParamAPP_Melody_5___Duration);
                        break;
                }

                configs.push_back(config);
                [[fallthrough]];
            }

            case 4: {

                melody.rtttl = reinterpret_cast<const char *>(ParamAPP_Melody_4);
                melody.pause_sec = ParamAPP_Melody_4_Melody_Pause;
                config.melody = melody;

                config.priority = 4; //TODO ADD PRIO FROM KNX

                switch(ParamAPP_Melody_4___Mode)
                {
                    case ModeType::MODE_TRIGGER:
                        config.mode = std::make_shared<TriggerMode>(APP_KoTrigger_Melody_4);
                        break;
                    case ModeType::MODE_SWITCH:
                        config.mode = std::make_shared<SwitchMode>(APP_KoSWITCH_Melody_4);
                        break;
                    case ModeType::MODE_VENTING_MONITOR:
                    default:
                        config.mode = std::make_shared<VentingMonitorMode>(
                            APP_KoSWITCH_Melody_4,
                            APP_KoTemperature_Melody_4,
                            ParamAPP_Melody_4___Lower_Temp_Exclusion_Limit,
                            ParamAPP_Melody_4___Upper_Temp_Exclusion_Limit,
                            ParamAPP_Melody_4___Duration);
                        break;
                }

                configs.push_back(config);
                [[fallthrough]];
            }

            case 3: {
                melody.rtttl = reinterpret_cast<const char *>(ParamAPP_Melody_3);
                melody.pause_sec = ParamAPP_Melody_3_Melody_Pause;
                config.melody = melody;

                config.priority = 3; //TODO ADD PRIO FROM KNX
                switch(ParamAPP_Melody_3___Mode)
                {
                    case ModeType::MODE_TRIGGER:
                        config.mode = std::make_shared<TriggerMode>(APP_KoTrigger_Melody_3);
                        break;
                    case ModeType::MODE_SWITCH:
                        config.mode = std::make_shared<SwitchMode>(APP_KoSWITCH_Melody_3);
                        break;
                    case ModeType::MODE_VENTING_MONITOR:
                    default:
                        config.mode = std::make_shared<VentingMonitorMode>(
                            APP_KoSWITCH_Melody_3,
                            APP_KoTemperature_Melody_3,
                            ParamAPP_Melody_3___Lower_Temp_Exclusion_Limit,
                            ParamAPP_Melody_3___Upper_Temp_Exclusion_Limit,
                            ParamAPP_Melody_3___Duration);
                        break;
                }

                configs.push_back(config);
                [[fallthrough]];
            }

            case 2: {
                melody.rtttl = reinterpret_cast<const char *>(ParamAPP_Melody_2);
                melody.pause_sec = ParamAPP_Melody_2_Melody_Pause;
                config.melody = melody;

                config.priority = 2; //TODO ADD PRIO FROM KNX
                switch(ParamAPP_Melody_2___Mode)
                {
                    case ModeType::MODE_TRIGGER:
                        config.mode = std::make_shared<TriggerMode>(APP_KoTrigger_Melody_2);
                        break;
                    case ModeType::MODE_SWITCH:
                        config.mode = std::make_shared<SwitchMode>(APP_KoSWITCH_Melody_2);
                        break;
                    case ModeType::MODE_VENTING_MONITOR:
                    default:
                        config.mode = std::make_shared<VentingMonitorMode>(
                            APP_KoSWITCH_Melody_2,
                            APP_KoTemperature_Melody_2,
                            ParamAPP_Melody_2___Lower_Temp_Exclusion_Limit,
                            ParamAPP_Melody_2___Upper_Temp_Exclusion_Limit,
                            ParamAPP_Melody_2___Duration);
                        break;
                }

                configs.push_back(config);
                [[fallthrough]];
            }

            case 1: {
                melody.rtttl = reinterpret_cast<const char *>(ParamAPP_Melody_1);
                melody.pause_sec = ParamAPP_Melody_1_Melody_Pause;
                config.melody = melody;

                config.priority = 1; //TODO ADD PRIO FROM KNX
                switch(ParamAPP_Melody_1___Mode)
                {
                    case ModeType::MODE_TRIGGER:
                        config.mode = std::make_shared<TriggerMode>(APP_KoTrigger_Melody_1);
                        break;
                    case ModeType::MODE_SWITCH:
                        config.mode = std::make_shared<SwitchMode>(APP_KoSWITCH_Melody_1);
                        break;
                    case ModeType::MODE_VENTING_MONITOR:
                    default:
                        config.mode = std::make_shared<VentingMonitorMode>(
                            APP_KoSWITCH_Melody_1,
                            APP_KoTemperature_Melody_1,
                            ParamAPP_Melody_1___Lower_Temp_Exclusion_Limit,
                            ParamAPP_Melody_1___Upper_Temp_Exclusion_Limit,
                            ParamAPP_Melody_1___Duration);
                        break;
                }

                configs.push_back(config);
            break;
            }
            default: {
                logWarning("Number of melodies unexpected, keeping system unconfigured!");
            }
        }
        return configs;
    }

    struct ApplicationVersion getApplicationVersion()
    {
        struct ApplicationVersion appVersion = {
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