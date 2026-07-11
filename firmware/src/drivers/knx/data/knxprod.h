#pragma once

#define paramDelay(time) (uint32_t)( \
            (time & 0xC000) == 0xC000 ? (time & 0x3FFF) * 100 : \
            (time & 0xC000) == 0x0000 ? (time & 0x3FFF) * 1000 : \
            (time & 0xC000) == 0x4000 ? (time & 0x3FFF) * 60000 : \
            (time & 0xC000) == 0x8000 ? ((time & 0x3FFF) > 1000 ? 3600000 : \
                                            (time & 0x3FFF) * 3600000 ) : 0 )
//--------------------Allgemein---------------------------
#define MAIN_OpenKnxId 0xAF
#define MAIN_ApplicationNumber 0x00
#define MAIN_ApplicationVersion 0x0A
#define MAIN_OrderNumber "TA-00002.1"
#define MAIN_ParameterSize 5
#define MAIN_MaxKoNumber 5


#define APP_Keep_Alive_Interval		0x0000
// Offset: 0, Size: 16 Bit (2 Byte), Text: Keep-Alive Interval
#define ParamAPP_Keep_Alive_Interval ((uint)((knx.paramWord(APP_Keep_Alive_Interval))))
#define APP_Polling_Interval		0x0002
// Offset: 2, Size: 16 Bit (2 Byte), Text: Abfrageintervall
#define ParamAPP_Polling_Interval ((uint)((knx.paramWord(APP_Polling_Interval))))
#define APP_Volume		0x0004
// Offset: 4, Size: 1 Bit, Text: Volumen
#define ParamAPP_Volume knx.paramBit(APP_Volume, 0)
#define APP_Flow		0x0004
// Offset: 4, BitOffset: 1, Size: 1 Bit, Text: Durchfluss
#define ParamAPP_Flow knx.paramBit(APP_Flow, 1)
#define APP_Temperature		0x0004
// Offset: 4, BitOffset: 2, Size: 1 Bit, Text: Temperatur
#define ParamAPP_Temperature knx.paramBit(APP_Temperature, 2)
#define APP_Battery_Life		0x0004
// Offset: 4, BitOffset: 3, Size: 1 Bit, Text: Battery Life
#define ParamAPP_Battery_Life knx.paramBit(APP_Battery_Life, 3)
//!< Number: 1, Text: Verbunden, Function: Verbunden
#define APP_KoLinked 1
#define KoAPP_Linked knx.getGroupObject(APP_KoLinked)
//!< Number: 2, Text: Durchfluss, Function: Durchfluss
#define APP_KoFlow 2
#define KoAPP_Flow knx.getGroupObject(APP_KoFlow)
//!< Number: 3, Text: Volumen, Function: Volumen
#define APP_KoVolume 3
#define KoAPP_Volume knx.getGroupObject(APP_KoVolume)
//!< Number: 4, Text: Temperatur, Function: Temperatur
#define APP_KoTemperature 4
#define KoAPP_Temperature knx.getGroupObject(APP_KoTemperature)
//!< Number: 5, Text: Verbleibende Batterielaufzeit, Function: Verbleibende Batterielaufzeit
#define APP_KoBattery_Life 5
#define KoAPP_Battery_Life knx.getGroupObject(APP_KoBattery_Life)

