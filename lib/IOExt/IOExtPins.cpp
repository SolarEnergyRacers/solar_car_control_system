//
// MCP23017 I/O Extension over I2C
//
#include <definitions.h>

#include <CarControl.h>
#include <Helper.h>
#include <IOExt.h>
#include <IOExtHandler.h>

extern Indicator indicator;
extern IOExt ioExt;
extern CarState carState;
extern CarControl carControl;
extern bool systemOk;

CarStatePin CarState::pins[] = {
    // port (device|pin), mode, value, oldValue, inited, timestamp, name, handler
    //
    // IOExtDev0-PortA
    {0x00, INPUT_PULLUP, 1, 1, false, 0l, PinBatOnOff, batteryOnOffHandler},
    {0x01, INPUT_PULLUP, 1, 1, false, 0l, PinPvOnOff, pvOnOffHandler},
    {0x02, INPUT, 1, 1, false, 0l, PinMcOnOff, mcOnOffHandler}, // dont pull because of z-diode
    {0x03, INPUT_PULLUP, 1, 1, false, 0l, PinEcoPower, ecoPowerHandler},

    {0x04, OUTPUT, 1, 1, false, 0l, PinDacLifeSign, NULL},
    {0x05, INPUT, 1, 1, false, 0l, PinFwdBwd, fwdBwdHandler}, // dont pull because of z-diode
    {0x06, INPUT_PULLUP, 1, 1, false, 0l, PinDUMMY06, NULL},
    {0x07, OUTPUT, 0, 0, false, 0l, PinFanOut, NULL},
    // IOExtDev0-PortB
    {0x08, OUTPUT, 0, 0, false, 0l, PinIndicatorOutLeft, NULL},
    {0x09, OUTPUT, 0, 0, false, 0l, PinIndicatorOutRight, NULL},
    {0x0a, OUTPUT, 0, 0, false, 0l, PinHornOut, NULL},
    {0x0b, OUTPUT, 0, 0, false, 0l, PinHeadLightOut, NULL},

    {0x0c, OUTPUT, 0, 0, false, 0l, PinLightOut, NULL},
    {0x0d, INPUT_PULLUP, 1, 1, false, 0l, PinBreakPedal, breakPedalHandler},
    {0x0e, INPUT_PULLUP, 1, 1, false, 0l, PinDUMMY14, NULL},
    {0x0f, INPUT_PULLUP, 1, 1, false, 0l, PinDUMMY15, NULL},
    // IOExtDev1-PortA
    {0x10, INPUT_PULLUP, 1, 1, false, 0l, PinIndicatorBtnLeft, indicatorHandler},
    {0x11, INPUT_PULLUP, 1, 1, false, 0l, PinHeadLight, headLightHandler},
    {0x12, INPUT_PULLUP, 1, 1, false, 0l, PinConstantModeOff, constantModeOffHandler},
    {0x13, INPUT_PULLUP, 1, 1, false, 0l, PinHorn, hornHandler},

    {0x14, INPUT_PULLUP, 1, 1, false, 0l, PinNextScreen, nextScreenHandler},
    {0x15, INPUT_PULLUP, 1, 1, false, 0l, PinConstantMode, constantModeHandler},
    {0x16, INPUT_PULLUP, 1, 1, false, 0l, PinIndicatorBtnRight, indicatorHandler},
    {0x17, INPUT_PULLUP, 1, 1, false, 0l, PinConstantModeOn, constantModeOnHandler}, // #SAFETY#: deceleration unlock const mode
    // IOExtDev1-PortB
    {0x18, INPUT_PULLUP, 1, 1, false, 0l, PinPaddleAdjust, paddleAdjustHandler}, // #SAFETY#: lock acceleration
    {0x19, INPUT_PULLUP, 1, 1, false, 0l, PinLight, lightHandler},
    {0x1a, INPUT_PULLUP, 1, 1, false, 0l, PinDUMMY33, NULL},
    {0x1b, INPUT_PULLUP, 1, 1, false, 0l, PinIncrease, increaseHandler},

    {0x1c, INPUT_PULLUP, 1, 1, false, 0l, PinDecrease, decreaseHandler},
    {0x1d, INPUT_PULLUP, 1, 1, false, 0l, PinSdCardDetect, sdCardDetectHandler},
    {0x1e, INPUT_PULLUP, 1, 1, false, 0l, PinDUMMY37, NULL},
    {0x1f, INPUT_PULLUP, 1, 1, false, 0l, PinDUMMY38, NULL}};