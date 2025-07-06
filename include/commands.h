#pragma once
#include <Arduino.h>

extern char cmd[14];

// Command construction function prototypes
void setCmd(const char* inputCmd, uint8_t inputParam = 0);
void setCmdNoReplace(const char* inputCmd);
void setCmdSwitch(const char* inputCmd1, const char* inputCmd2, uint8_t switchIndex);
void setCmdStep(const char* inputCmd, uint8_t param, uint8_t maxValue);
void setCmdToggle(const char* inputCmd, uint8_t maxValue, uint8_t switchIndex);
void sendCmd(HardwareSerialIMXRT& s);

// commands (wip) for panasonic mx30/50/70:

#define MX30_A_B_MIX_LEVEL "VMM:~0" // "VMP:~a*b*c000"

#define MX30_A_BUS_MOSAIC_OFF "VDE:AMSOF"
#define MX30_A_BUS_MOSAIC_STEP "VDE:AMS~0"
#define MX30_THRESHOLD_LUM_KEY "VKL:~0"
#define MX30_CENTER_WIPE_X "VPS:N~d*e" //"VMP:*a~b*c000"
#define MX30_CENTER_WIPE_Y "VPS:N*d~e" // "VMP:*a*b~c000"
#define MX30_SCENE_GRABER_ON "VSB:N"
#define MX30_SCENE_GRABER_OFF "VSB:F"

#define MX30_WIPE "VWP:~0ZMF" // max 26
// #define MX30_WIPE_NUMBER "VWN:1420"
#define MX30_KEY_SLICE "VKS:~0F"

#define MX30_STROBO "VDE:ASR~" // max 62
#define MX30_MOSAIC "VDE:AMS~" // max 7
#define MX30_PAINT "VDE:APN~" // max 30

#define MX30_A_BUS_STROBO_OFF "VDE:ASROF"
#define MX30_A_BUS_MOSAIC_OFF "VDE:AMSOF"
#define MX30_A_BUS_PAINT_OFF "VDE:APNF"
#define MX30_B_BUS_STROBO_OFF "VDE:BSROF"
#define MX30_B_BUS_MOSAIC_OFF "VDE:BMSOF"
#define MX30_B_BUS_PAINT_OFF "VDE:BPNF"

#define MX30_A_BUS_NEGATIVE_OFF "VDE:ANGF"
#define MX30_A_BUS_NEGATIVE_ON "VDE:ANGN"
#define MX30_B_BUS_NEGATIVE_OFF "VDE:BNGF"
#define MX30_B_BUS_NEGATIVE_ON "VDE:BNGN"

#define MX30_COLOR_CORRECT_X "VCC:T~f*g"
#define MX30_COLOR_CORRECT_Y "VCC:T*f~g"
#define MX30_COLOR_CORRECT_GAIN "VCG:T~0"

#define MX30_A_BUS_COLOR_CORRECT_OFF "VCC:AOF"
#define MX30_B_BUS_COLOR_CORRECT_OFF "VCC:BOF"
#define MX30_A_BUS_COLOR_CORRECT_ON "VCC:A*f*g"
#define MX30_B_BUS_COLOR_CORRECT_ON "VCC:B*f*g"


#define MX30_A_BUS_EFFECT_ON "VDE:AON"
#define MX30_A_BUS_EFFECT_OFF "VDE:AOF"
#define MX30_B_BUS_EFFECT_ON "VDE:BON"
#define MX30_B_BUS_EFFECT_OFF "VDE:BOF"


#define MX30_A_BUS_SOURCE_1 "VCP:A1"
#define MX30_A_BUS_SOURCE_2 "VCP:A2"
#define MX30_A_BUS_SOURCE_3 "VCP:A3"
#define MX30_A_BUS_SOURCE_4 "VCP:A4"
#define MX30_A_BUS_BACK_COLOR "VCP:AB"
#define MX30_B_BUS_SOURCE_1 "VCP:B1"
#define MX30_B_BUS_SOURCE_2 "VCP:B2"
#define MX30_B_BUS_SOURCE_3 "VCP:B3"
#define MX30_B_BUS_SOURCE_4 "VCP:B4"
#define MX30_B_BUS_BACK_COLOR "VCP:BB"