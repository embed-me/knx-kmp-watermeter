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
#define MAIN_ApplicationVersion 0x3D
#define MAIN_OrderNumber "TA-00002.1"
#define MAIN_ParameterSize 4129
#define MAIN_MaxKoNumber 24


#define APP_Number_Melodies		0x0000
#define APP_Number_Melodies_Shift	4
#define APP_Number_Melodies_Mask	0x000F
// Offset: 0, Size: 4 Bit, Text: Anzahl Melodien
#define ParamAPP_Number_Melodies ((uint)((knx.paramByte(APP_Number_Melodies) >> APP_Number_Melodies_Shift) & APP_Number_Melodies_Mask))
#define APP_Melody_1_Melody_Pause		0x0001
#define APP_Melody_1_Melody_Pause_Shift	2
#define APP_Melody_1_Melody_Pause_Mask	0x003F
// Offset: 1, Size: 6 Bit, Text: Pause zwischen Wiederholungen
#define ParamAPP_Melody_1_Melody_Pause ((uint)((knx.paramByte(APP_Melody_1_Melody_Pause) >> APP_Melody_1_Melody_Pause_Shift) & APP_Melody_1_Melody_Pause_Mask))
#define APP_Melody_2_Melody_Pause		0x0002
#define APP_Melody_2_Melody_Pause_Shift	2
#define APP_Melody_2_Melody_Pause_Mask	0x003F
// Offset: 2, Size: 6 Bit, Text: Pause zwischen Wiederholungen
#define ParamAPP_Melody_2_Melody_Pause ((uint)((knx.paramByte(APP_Melody_2_Melody_Pause) >> APP_Melody_2_Melody_Pause_Shift) & APP_Melody_2_Melody_Pause_Mask))
#define APP_Melody_3_Melody_Pause		0x0003
#define APP_Melody_3_Melody_Pause_Shift	2
#define APP_Melody_3_Melody_Pause_Mask	0x003F
// Offset: 3, Size: 6 Bit, Text: Pause zwischen Wiederholungen
#define ParamAPP_Melody_3_Melody_Pause ((uint)((knx.paramByte(APP_Melody_3_Melody_Pause) >> APP_Melody_3_Melody_Pause_Shift) & APP_Melody_3_Melody_Pause_Mask))
#define APP_Melody_4_Melody_Pause		0x0004
#define APP_Melody_4_Melody_Pause_Shift	2
#define APP_Melody_4_Melody_Pause_Mask	0x003F
// Offset: 4, Size: 6 Bit, Text: Pause zwischen Wiederholungen
#define ParamAPP_Melody_4_Melody_Pause ((uint)((knx.paramByte(APP_Melody_4_Melody_Pause) >> APP_Melody_4_Melody_Pause_Shift) & APP_Melody_4_Melody_Pause_Mask))
#define APP_Melody_5_Melody_Pause		0x0005
#define APP_Melody_5_Melody_Pause_Shift	2
#define APP_Melody_5_Melody_Pause_Mask	0x003F
// Offset: 5, Size: 6 Bit, Text: Pause zwischen Wiederholungen
#define ParamAPP_Melody_5_Melody_Pause ((uint)((knx.paramByte(APP_Melody_5_Melody_Pause) >> APP_Melody_5_Melody_Pause_Shift) & APP_Melody_5_Melody_Pause_Mask))
#define APP_Melody_6_Melody_Pause		0x0006
#define APP_Melody_6_Melody_Pause_Shift	2
#define APP_Melody_6_Melody_Pause_Mask	0x003F
// Offset: 6, Size: 6 Bit, Text: Pause zwischen Wiederholungen
#define ParamAPP_Melody_6_Melody_Pause ((uint)((knx.paramByte(APP_Melody_6_Melody_Pause) >> APP_Melody_6_Melody_Pause_Shift) & APP_Melody_6_Melody_Pause_Mask))
#define APP_Melody_7_Melody_Pause		0x0007
#define APP_Melody_7_Melody_Pause_Shift	2
#define APP_Melody_7_Melody_Pause_Mask	0x003F
// Offset: 7, Size: 6 Bit, Text: Pause zwischen Wiederholungen
#define ParamAPP_Melody_7_Melody_Pause ((uint)((knx.paramByte(APP_Melody_7_Melody_Pause) >> APP_Melody_7_Melody_Pause_Shift) & APP_Melody_7_Melody_Pause_Mask))
#define APP_Melody_8_Melody_Pause		0x0008
#define APP_Melody_8_Melody_Pause_Shift	2
#define APP_Melody_8_Melody_Pause_Mask	0x003F
// Offset: 8, Size: 6 Bit, Text: Pause zwischen Wiederholungen
#define ParamAPP_Melody_8_Melody_Pause ((uint)((knx.paramByte(APP_Melody_8_Melody_Pause) >> APP_Melody_8_Melody_Pause_Shift) & APP_Melody_8_Melody_Pause_Mask))
#define APP_Melody_1		0x0009
// Offset: 9, Size: 4096 Bit (512 Byte), Text: RTTTL format
#define ParamAPP_Melody_1 knx.paramData(APP_Melody_1)
#define APP_Melody_2		0x0209
// Offset: 521, Size: 4096 Bit (512 Byte), Text: RTTTL format
#define ParamAPP_Melody_2 knx.paramData(APP_Melody_2)
#define APP_Melody_3		0x0409
// Offset: 1033, Size: 4096 Bit (512 Byte), Text: RTTTL format
#define ParamAPP_Melody_3 knx.paramData(APP_Melody_3)
#define APP_Melody_4		0x0609
// Offset: 1545, Size: 4096 Bit (512 Byte), Text: RTTTL format
#define ParamAPP_Melody_4 knx.paramData(APP_Melody_4)
#define APP_Melody_5		0x0809
// Offset: 2057, Size: 4096 Bit (512 Byte), Text: RTTTL format
#define ParamAPP_Melody_5 knx.paramData(APP_Melody_5)
#define APP_Melody_6		0x0A09
// Offset: 2569, Size: 4096 Bit (512 Byte), Text: RTTTL format
#define ParamAPP_Melody_6 knx.paramData(APP_Melody_6)
#define APP_Melody_7		0x0C09
// Offset: 3081, Size: 4096 Bit (512 Byte), Text: RTTTL format
#define ParamAPP_Melody_7 knx.paramData(APP_Melody_7)
#define APP_Melody_8		0x0E09
// Offset: 3593, Size: 4096 Bit (512 Byte), Text: RTTTL format
#define ParamAPP_Melody_8 knx.paramData(APP_Melody_8)
#define APP_Melody_1___Mode		0x0000
#define APP_Melody_1___Mode_Shift	2
#define APP_Melody_1___Mode_Mask	0x0003
// Offset: 0, BitOffset: 4, Size: 2 Bit, Text: Melodie Mode
#define ParamAPP_Melody_1___Mode ((uint)((knx.paramByte(APP_Melody_1___Mode) >> APP_Melody_1___Mode_Shift) & APP_Melody_1___Mode_Mask))
#define APP_Melody_2___Mode		0x0000
#define APP_Melody_2___Mode_Mask	0x0003
// Offset: 0, BitOffset: 6, Size: 2 Bit, Text: Melodie Mode
#define ParamAPP_Melody_2___Mode ((uint)((knx.paramByte(APP_Melody_2___Mode)) & APP_Melody_2___Mode_Mask))
#define APP_Melody_3___Mode		0x0001
#define APP_Melody_3___Mode_Mask	0x0003
// Offset: 1, BitOffset: 6, Size: 2 Bit, Text: Melodie Mode
#define ParamAPP_Melody_3___Mode ((uint)((knx.paramByte(APP_Melody_3___Mode)) & APP_Melody_3___Mode_Mask))
#define APP_Melody_4___Mode		0x0002
#define APP_Melody_4___Mode_Mask	0x0003
// Offset: 2, BitOffset: 6, Size: 2 Bit, Text: Melodie Mode
#define ParamAPP_Melody_4___Mode ((uint)((knx.paramByte(APP_Melody_4___Mode)) & APP_Melody_4___Mode_Mask))
#define APP_Melody_5___Mode		0x0003
#define APP_Melody_5___Mode_Mask	0x0003
// Offset: 3, BitOffset: 6, Size: 2 Bit, Text: Melodie Mode
#define ParamAPP_Melody_5___Mode ((uint)((knx.paramByte(APP_Melody_5___Mode)) & APP_Melody_5___Mode_Mask))
#define APP_Melody_6___Mode		0x0004
#define APP_Melody_6___Mode_Mask	0x0003
// Offset: 4, BitOffset: 6, Size: 2 Bit, Text: Melodie Mode
#define ParamAPP_Melody_6___Mode ((uint)((knx.paramByte(APP_Melody_6___Mode)) & APP_Melody_6___Mode_Mask))
#define APP_Melody_7___Mode		0x0005
#define APP_Melody_7___Mode_Mask	0x0003
// Offset: 5, BitOffset: 6, Size: 2 Bit, Text: Melodie Mode
#define ParamAPP_Melody_7___Mode ((uint)((knx.paramByte(APP_Melody_7___Mode)) & APP_Melody_7___Mode_Mask))
#define APP_Melody_8___Mode		0x0006
#define APP_Melody_8___Mode_Mask	0x0003
// Offset: 6, BitOffset: 6, Size: 2 Bit, Text: Melodie Mode
#define ParamAPP_Melody_8___Mode ((uint)((knx.paramByte(APP_Melody_8___Mode)) & APP_Melody_8___Mode_Mask))
#define APP_Melody_1___Lower_Temp_Exclusion_Limit		0x1009
// Offset: 4105, Size: 8 Bit (1 Byte), Text: Unterer Temperaturgrenzwert
#define ParamAPP_Melody_1___Lower_Temp_Exclusion_Limit ((int8_t)((knx.paramByte(APP_Melody_1___Lower_Temp_Exclusion_Limit))))
#define APP_Melody_2___Lower_Temp_Exclusion_Limit		0x100A
// Offset: 4106, Size: 8 Bit (1 Byte), Text: Unterer Temperaturgrenzwert
#define ParamAPP_Melody_2___Lower_Temp_Exclusion_Limit ((int8_t)((knx.paramByte(APP_Melody_2___Lower_Temp_Exclusion_Limit))))
#define APP_Melody_3___Lower_Temp_Exclusion_Limit		0x100B
// Offset: 4107, Size: 8 Bit (1 Byte), Text: Unterer Temperaturgrenzwert
#define ParamAPP_Melody_3___Lower_Temp_Exclusion_Limit ((int8_t)((knx.paramByte(APP_Melody_3___Lower_Temp_Exclusion_Limit))))
#define APP_Melody_4___Lower_Temp_Exclusion_Limit		0x100C
// Offset: 4108, Size: 8 Bit (1 Byte), Text: Unterer Temperaturgrenzwert
#define ParamAPP_Melody_4___Lower_Temp_Exclusion_Limit ((int8_t)((knx.paramByte(APP_Melody_4___Lower_Temp_Exclusion_Limit))))
#define APP_Melody_5___Lower_Temp_Exclusion_Limit		0x100D
// Offset: 4109, Size: 8 Bit (1 Byte), Text: Unterer Temperaturgrenzwert
#define ParamAPP_Melody_5___Lower_Temp_Exclusion_Limit ((int8_t)((knx.paramByte(APP_Melody_5___Lower_Temp_Exclusion_Limit))))
#define APP_Melody_6___Lower_Temp_Exclusion_Limit		0x100E
// Offset: 4110, Size: 8 Bit (1 Byte), Text: Unterer Temperaturgrenzwert
#define ParamAPP_Melody_6___Lower_Temp_Exclusion_Limit ((int8_t)((knx.paramByte(APP_Melody_6___Lower_Temp_Exclusion_Limit))))
#define APP_Melody_7___Lower_Temp_Exclusion_Limit		0x100F
// Offset: 4111, Size: 8 Bit (1 Byte), Text: Unterer Temperaturgrenzwert
#define ParamAPP_Melody_7___Lower_Temp_Exclusion_Limit ((int8_t)((knx.paramByte(APP_Melody_7___Lower_Temp_Exclusion_Limit))))
#define APP_Melody_8___Lower_Temp_Exclusion_Limit		0x1010
// Offset: 4112, Size: 8 Bit (1 Byte), Text: Unterer Temperaturgrenzwert
#define ParamAPP_Melody_8___Lower_Temp_Exclusion_Limit ((int8_t)((knx.paramByte(APP_Melody_8___Lower_Temp_Exclusion_Limit))))
#define APP_Melody_1___Upper_Temp_Exclusion_Limit		0x1011
// Offset: 4113, Size: 8 Bit (1 Byte), Text: Oberer Temperaturgrenzwert
#define ParamAPP_Melody_1___Upper_Temp_Exclusion_Limit ((int8_t)((knx.paramByte(APP_Melody_1___Upper_Temp_Exclusion_Limit))))
#define APP_Melody_2___Upper_Temp_Exclusion_Limit		0x1012
// Offset: 4114, Size: 8 Bit (1 Byte), Text: Oberer Temperaturgrenzwert
#define ParamAPP_Melody_2___Upper_Temp_Exclusion_Limit ((int8_t)((knx.paramByte(APP_Melody_2___Upper_Temp_Exclusion_Limit))))
#define APP_Melody_3___Upper_Temp_Exclusion_Limit		0x1013
// Offset: 4115, Size: 8 Bit (1 Byte), Text: Oberer Temperaturgrenzwert
#define ParamAPP_Melody_3___Upper_Temp_Exclusion_Limit ((int8_t)((knx.paramByte(APP_Melody_3___Upper_Temp_Exclusion_Limit))))
#define APP_Melody_4___Upper_Temp_Exclusion_Limit		0x1014
// Offset: 4116, Size: 8 Bit (1 Byte), Text: Oberer Temperaturgrenzwert
#define ParamAPP_Melody_4___Upper_Temp_Exclusion_Limit ((int8_t)((knx.paramByte(APP_Melody_4___Upper_Temp_Exclusion_Limit))))
#define APP_Melody_5___Upper_Temp_Exclusion_Limit		0x1015
// Offset: 4117, Size: 8 Bit (1 Byte), Text: Oberer Temperaturgrenzwert
#define ParamAPP_Melody_5___Upper_Temp_Exclusion_Limit ((int8_t)((knx.paramByte(APP_Melody_5___Upper_Temp_Exclusion_Limit))))
#define APP_Melody_6___Upper_Temp_Exclusion_Limit		0x1016
// Offset: 4118, Size: 8 Bit (1 Byte), Text: Oberer Temperaturgrenzwert
#define ParamAPP_Melody_6___Upper_Temp_Exclusion_Limit ((int8_t)((knx.paramByte(APP_Melody_6___Upper_Temp_Exclusion_Limit))))
#define APP_Melody_7___Upper_Temp_Exclusion_Limit		0x1017
// Offset: 4119, Size: 8 Bit (1 Byte), Text: Oberer Temperaturgrenzwert
#define ParamAPP_Melody_7___Upper_Temp_Exclusion_Limit ((int8_t)((knx.paramByte(APP_Melody_7___Upper_Temp_Exclusion_Limit))))
#define APP_Melody_8___Upper_Temp_Exclusion_Limit		0x1018
// Offset: 4120, Size: 8 Bit (1 Byte), Text: Oberer Temperaturgrenzwert
#define ParamAPP_Melody_8___Upper_Temp_Exclusion_Limit ((int8_t)((knx.paramByte(APP_Melody_8___Upper_Temp_Exclusion_Limit))))
#define APP_Melody_1___Duration		0x1019
#define APP_Melody_1___Duration_Shift	2
#define APP_Melody_1___Duration_Mask	0x003F
// Offset: 4121, Size: 6 Bit, Text: Lüftzeit bis aktion
#define ParamAPP_Melody_1___Duration ((uint)((knx.paramByte(APP_Melody_1___Duration) >> APP_Melody_1___Duration_Shift) & APP_Melody_1___Duration_Mask))
#define APP_Melody_2___Duration		0x101A
#define APP_Melody_2___Duration_Shift	2
#define APP_Melody_2___Duration_Mask	0x003F
// Offset: 4122, Size: 6 Bit, Text: Lüftzeit bis aktion
#define ParamAPP_Melody_2___Duration ((uint)((knx.paramByte(APP_Melody_2___Duration) >> APP_Melody_2___Duration_Shift) & APP_Melody_2___Duration_Mask))
#define APP_Melody_3___Duration		0x101B
#define APP_Melody_3___Duration_Shift	2
#define APP_Melody_3___Duration_Mask	0x003F
// Offset: 4123, Size: 6 Bit, Text: Lüftzeit bis aktion
#define ParamAPP_Melody_3___Duration ((uint)((knx.paramByte(APP_Melody_3___Duration) >> APP_Melody_3___Duration_Shift) & APP_Melody_3___Duration_Mask))
#define APP_Melody_4___Duration		0x101C
#define APP_Melody_4___Duration_Shift	2
#define APP_Melody_4___Duration_Mask	0x003F
// Offset: 4124, Size: 6 Bit, Text: Lüftzeit bis aktion
#define ParamAPP_Melody_4___Duration ((uint)((knx.paramByte(APP_Melody_4___Duration) >> APP_Melody_4___Duration_Shift) & APP_Melody_4___Duration_Mask))
#define APP_Melody_5___Duration		0x101D
#define APP_Melody_5___Duration_Shift	2
#define APP_Melody_5___Duration_Mask	0x003F
// Offset: 4125, Size: 6 Bit, Text: Lüftzeit bis aktion
#define ParamAPP_Melody_5___Duration ((uint)((knx.paramByte(APP_Melody_5___Duration) >> APP_Melody_5___Duration_Shift) & APP_Melody_5___Duration_Mask))
#define APP_Melody_6___Duration		0x101E
#define APP_Melody_6___Duration_Shift	2
#define APP_Melody_6___Duration_Mask	0x003F
// Offset: 4126, Size: 6 Bit, Text: Lüftzeit bis aktion
#define ParamAPP_Melody_6___Duration ((uint)((knx.paramByte(APP_Melody_6___Duration) >> APP_Melody_6___Duration_Shift) & APP_Melody_6___Duration_Mask))
#define APP_Melody_7___Duration		0x101F
#define APP_Melody_7___Duration_Shift	2
#define APP_Melody_7___Duration_Mask	0x003F
// Offset: 4127, Size: 6 Bit, Text: Lüftzeit bis aktion
#define ParamAPP_Melody_7___Duration ((uint)((knx.paramByte(APP_Melody_7___Duration) >> APP_Melody_7___Duration_Shift) & APP_Melody_7___Duration_Mask))
#define APP_Melody_8___Duration		0x1020
#define APP_Melody_8___Duration_Shift	2
#define APP_Melody_8___Duration_Mask	0x003F
// Offset: 4128, Size: 6 Bit, Text: Lüftzeit bis aktion
#define ParamAPP_Melody_8___Duration ((uint)((knx.paramByte(APP_Melody_8___Duration) >> APP_Melody_8___Duration_Shift) & APP_Melody_8___Duration_Mask))
//!< Number: 1, Text: Melodie 1 Abspielen, Function: Melodie 1 Abspielen
#define APP_KoTrigger_Melody_1 1
#define KoAPP_Trigger_Melody_1 knx.getGroupObject(APP_KoTrigger_Melody_1)
//!< Number: 2, Text: Melodie 2 Abspielen, Function: Melodie 2 Abspielen
#define APP_KoTrigger_Melody_2 2
#define KoAPP_Trigger_Melody_2 knx.getGroupObject(APP_KoTrigger_Melody_2)
//!< Number: 3, Text: Melodie 3 Abspielen, Function: Melodie 3 Abspielen
#define APP_KoTrigger_Melody_3 3
#define KoAPP_Trigger_Melody_3 knx.getGroupObject(APP_KoTrigger_Melody_3)
//!< Number: 4, Text: Melodie 4 Abspielen, Function: Melodie 4 Abspielen
#define APP_KoTrigger_Melody_4 4
#define KoAPP_Trigger_Melody_4 knx.getGroupObject(APP_KoTrigger_Melody_4)
//!< Number: 5, Text: Melodie 5 Abspielen, Function: Melodie 5 Abspielen
#define APP_KoTrigger_Melody_5 5
#define KoAPP_Trigger_Melody_5 knx.getGroupObject(APP_KoTrigger_Melody_5)
//!< Number: 6, Text: Melodie 6 Abspielen, Function: Melodie 6 Abspielen
#define APP_KoTrigger_Melody_6 6
#define KoAPP_Trigger_Melody_6 knx.getGroupObject(APP_KoTrigger_Melody_6)
//!< Number: 7, Text: Melodie 7 Abspielen, Function: Melodie 7 Abspielen
#define APP_KoTrigger_Melody_7 7
#define KoAPP_Trigger_Melody_7 knx.getGroupObject(APP_KoTrigger_Melody_7)
//!< Number: 8, Text: Melodie 8 Abspielen, Function: Melodie 8 Abspielen
#define APP_KoTrigger_Melody_8 8
#define KoAPP_Trigger_Melody_8 knx.getGroupObject(APP_KoTrigger_Melody_8)
//!< Number: 9, Text: Melodie 1 Abspielen, Function: Melodie 1 Abspielen
#define APP_KoSWITCH_Melody_1 9
#define KoAPP_SWITCH_Melody_1 knx.getGroupObject(APP_KoSWITCH_Melody_1)
//!< Number: 10, Text: Melodie 2 Abspielen, Function: Melodie 2 Abspielen
#define APP_KoSWITCH_Melody_2 10
#define KoAPP_SWITCH_Melody_2 knx.getGroupObject(APP_KoSWITCH_Melody_2)
//!< Number: 11, Text: Melodie 3 Abspielen, Function: Melodie 3 Abspielen
#define APP_KoSWITCH_Melody_3 11
#define KoAPP_SWITCH_Melody_3 knx.getGroupObject(APP_KoSWITCH_Melody_3)
//!< Number: 12, Text: Melodie 4 Abspielen, Function: Melodie 4 Abspielen
#define APP_KoSWITCH_Melody_4 12
#define KoAPP_SWITCH_Melody_4 knx.getGroupObject(APP_KoSWITCH_Melody_4)
//!< Number: 13, Text: Melodie 5 Abspielen, Function: Melodie 5 Abspielen
#define APP_KoSWITCH_Melody_5 13
#define KoAPP_SWITCH_Melody_5 knx.getGroupObject(APP_KoSWITCH_Melody_5)
//!< Number: 14, Text: Melodie 6 Abspielen, Function: Melodie 6 Abspielen
#define APP_KoSWITCH_Melody_6 14
#define KoAPP_SWITCH_Melody_6 knx.getGroupObject(APP_KoSWITCH_Melody_6)
//!< Number: 15, Text: Melodie 7 Abspielen, Function: Melodie 7 Abspielen
#define APP_KoSWITCH_Melody_7 15
#define KoAPP_SWITCH_Melody_7 knx.getGroupObject(APP_KoSWITCH_Melody_7)
//!< Number: 16, Text: Melodie 8 Abspielen, Function: Melodie 8 Abspielen
#define APP_KoSWITCH_Melody_8 16
#define KoAPP_SWITCH_Melody_8 knx.getGroupObject(APP_KoSWITCH_Melody_8)
//!< Number: 17, Text: Temperature Melody 1, Function: Temperatur
#define APP_KoTemperature_Melody_1 17
#define KoAPP_Temperature_Melody_1 knx.getGroupObject(APP_KoTemperature_Melody_1)
//!< Number: 18, Text: Temperature Melody 2, Function: Temperatur
#define APP_KoTemperature_Melody_2 18
#define KoAPP_Temperature_Melody_2 knx.getGroupObject(APP_KoTemperature_Melody_2)
//!< Number: 19, Text: Temperature Melody 3, Function: Temperatur
#define APP_KoTemperature_Melody_3 19
#define KoAPP_Temperature_Melody_3 knx.getGroupObject(APP_KoTemperature_Melody_3)
//!< Number: 20, Text: Temperature Melody 4, Function: Temperatur
#define APP_KoTemperature_Melody_4 20
#define KoAPP_Temperature_Melody_4 knx.getGroupObject(APP_KoTemperature_Melody_4)
//!< Number: 21, Text: Temperature Melody 5, Function: Temperatur
#define APP_KoTemperature_Melody_5 21
#define KoAPP_Temperature_Melody_5 knx.getGroupObject(APP_KoTemperature_Melody_5)
//!< Number: 22, Text: Temperature Melody 6, Function: Temperatur
#define APP_KoTemperature_Melody_6 22
#define KoAPP_Temperature_Melody_6 knx.getGroupObject(APP_KoTemperature_Melody_6)
//!< Number: 23, Text: Temperature Melody 7, Function: Temperatur
#define APP_KoTemperature_Melody_7 23
#define KoAPP_Temperature_Melody_7 knx.getGroupObject(APP_KoTemperature_Melody_7)
//!< Number: 24, Text: Temperature Melody 8, Function: Temperatur
#define APP_KoTemperature_Melody_8 24
#define KoAPP_Temperature_Melody_8 knx.getGroupObject(APP_KoTemperature_Melody_8)

