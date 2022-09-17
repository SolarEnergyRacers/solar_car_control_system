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
    // port (device|pin), mask, mode, value, oldValue, inited, timestamp, name, handler
    //
    // IOExtDev0-PortA (main board)
    {0x00, 0x01, INPUT_PULLUP, 1, 1, false, 0l, PinBatOnOff, batteryOnOffHandler},
    {0x01, 0x02, INPUT_PULLUP, 1, 1, false, 0l, PinPvOnOff, pvOnOffHandler},
    {0x02, 0x04, INPUT, 1, 1, false, 0l, PinMcOnOff, mcOnOffHandler}, // dont pull because of z-diode
    {0x03, 0x08, INPUT_PULLUP, 1, 1, false, 0l, PinEcoPower, ecoPowerHandler},
    //
    {0x04, 0x10, OUTPUT, 1, 1, false, 0l, PinDacLifeSign, NULL},
    {0x05, 0x20, INPUT, 1, 1, false, 0l, PinFwdBwd, fwdBwdHandler}, // dont pull because of z-diode
    {0x06, 0x40, OUTPUT, 0, 0, false, 0l, PinGreenLightOut, NULL},
    {0x07, 0x80, OUTPUT, 0, 0, false, 0l, PinFanOut, NULL},
    // IOExtDev0-PortB (main board)
    {0x08, 0x01, OUTPUT, 0, 0, false, 0l, PinIndicatorOutLeft, NULL},
    {0x09, 0x02, OUTPUT, 0, 0, false, 0l, PinIndicatorOutRight, NULL},
    {0x0a, 0x04, OUTPUT, 0, 0, false, 0l, PinHornOut, NULL},
    {0x0b, 0x08, OUTPUT, 0, 0, false, 0l, PinHeadLightOut, NULL},
    //
    {0x0c, 0x10, OUTPUT, 0, 0, false, 0l, PinLightOut, NULL},
    {0x0d, 0x20, INPUT_PULLUP, 1, 1, false, 0l, PinBreakPedal, breakPedalHandler},
    {0x0e, 0x40, INPUT_PULLUP, 1, 1, false, 0l, PinDUMMY14, NULL},
    {0x0f, 0x80, INPUT_PULLUP, 1, 1, false, 0l, PinDUMMY15, NULL},

    // IOExtDev1-PortA (Steering wheel)
    {0x10, 0x01, INPUT_PULLUP, 1, 1, false, 0l, PinIndicatorBtnLeft, indicatorHandler},
    {0x11, 0x02, INPUT_PULLUP, 1, 1, false, 0l, PinHeadLight, headLightHandler},
    {0x12, 0x04, INPUT_PULLUP, 1, 1, false, 0l, PinConstantModeOff, constantModeOffHandler},
    {0x13, 0x08, INPUT_PULLUP, 1, 1, false, 0l, PinHorn, hornHandler},
    //
    {0x14, 0x10, INPUT_PULLUP, 1, 1, false, 0l, PinNextScreen, nextScreenHandler},
    {0x15, 0x20, INPUT_PULLUP, 1, 1, false, 0l, PinConstantMode, constantModeHandler},
    {0x16, 0x40, INPUT_PULLUP, 1, 1, false, 0l, PinIndicatorBtnRight, indicatorHandler},
    {0x17, 0x80, INPUT_PULLUP, 1, 1, false, 0l, PinConstantModeOn, constantModeOnHandler}, // #SAFETY#: deceleration unlock const mode
    // IOExtDev1-PortB (Steering wheel)
    {0x18, 0x01, INPUT_PULLUP, 1, 1, false, 0l, PinPaddleAdjust, paddleAdjustHandler}, // #SAFETY#: lock acceleration
    {0x19, 0x02, INPUT_PULLUP, 1, 1, false, 0l, PinLight, lightHandler},
    {0x1a, 0x04, INPUT_PULLUP, 1, 1, false, 0l, PinControlMode, controlModeHandler},
    {0x1b, 0x08, INPUT_PULLUP, 1, 1, false, 0l, PinIncrease, increaseHandler},
    //
    {0x1c, 0x10, INPUT_PULLUP, 1, 1, false, 0l, PinDecrease, decreaseHandler},
    {0x1d, 0x20, INPUT_PULLUP, 1, 1, false, 0l, PinSdCardDetect, sdCardDetectHandler},
    {0x1e, 0x40, INPUT_PULLUP, 1, 1, false, 0l, PinDUMMY37, NULL},
    {0x1f, 0x80, INPUT_PULLUP, 1, 1, false, 0l, PinDUMMY38, NULL}
    ////
};