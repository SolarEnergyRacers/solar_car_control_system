/*
 * MCP23017 I/O Extension over I2C  !!! UNTESTED !!!
 */

#ifndef SER_IOEXT2_H
#define SER_IOEXT2_H

#include <definitions.h>
#if IOEXT2_ON

#include <MCP23017.h>
#include <list>
#include <map>
#include <string>

#include <abstract_task.h>

#include <CarState.h>
#include <CarStatePin.h>
#include <Indicator.h>

// IOExtDevX
#define PinBatOnOff "BatOnOff"
#define PinPvOnOff "PvOnOff"
#define PinMcOnOff "McOnOff"
#define PinEcoPower "EcoPower"
#define PinDUMMY04 "DUMMY04"
#define PinFwdBwd "FwdBwd"
#define PinDUMMY06 "DUMMY06"
#define PinRelais11 "Relais11"
#define PinIndicatorOutLeft "IndicatorOutLeft"
#define PinIndicatorOutRight "IndicatorOutRight"
#define PinFanOut "PinFanOut"
#define PinHornOut "PinHornOut"
#define PinLightOut "PinLightOut"
#define PinBreakPedal "BreakPedal"
#define PinHeadLightOut "PinHeadLightOut"
#define PinDUMMY14 "DUMMY14"
#define PinDUMMY15 "DUMMY15"
#define PinIndicatorBtnLeft "IndicatorBtnLeft"
#define PinIndicatorBtnRight "IndicatorBtnRight"
#define PinLight "Light"
#define PinHeadLight "HeadLight"
#define PinConstantMode "ConstantMode"
#define PinConstantModeOn "ConstantModeOn"
#define PinHorn "Horn"
#define PinNextScreen "NextScreen"
#define PinDUMMY31 "DUMMY31"
#define PinReserve1 "Reserve1"
#define PinDUMMY33 "DUMMY33"
#define PinPaddleAdjust "PaddleAdjust"
#define PinDUMMY35 "DUMMY35"
#define PinDUMMY36 "DUMMY36"
#define PinDUMMY37 "DUMMY37"
#define PinDUMMY38 "DUMMY38"

// known pin handler
// the handler must its bit copy to oldValue
void batteryOnOffHandler();
void mcOnOffHandler();
void pvOnOffHandler();
void fwdBwdHandler();
void ecoPowerHandler();
void breakPedalHandler();
void indicatorHandler();
void hornHandler();
void nextScreenHandler();
void lightHandler();
void headLightHandler();
void constantModeHandler();
void constantModeOnOffHandler();
void paddleAdjustHandler();
// end pin handler

class IOExt2 : public abstract_task {
public:
  string getName(void) { return "IOExt2"; };

  void init(void);
  void re_init(void);
  void exit(void);
  void task(void);

  void setPort(int port, bool value);
  int getPort(int port);

  void readAll(bool deltaOnly = false);

  static int getIdx(int devNr, int pin) { return devNr * 16 + pin; };
  static int getIdx(int port) { return (port >> 4) * 16 + (port & 0x0F); };

private:
  // static volatile bool ioInterruptRequest;
  bool isInInterruptHandler = false;

  void setPortMode(int port, uint8_t mode);
  void handleIoInterrupt();
  // static void ioExt_interrupt_handler() { ioInterruptRequest = true; };

  MCP23017 IOExtDevs[2] = {
      MCP23017(I2C_ADDRESS_MCP23017_IOExt0), // Pins 00-15: Main board
      MCP23017(I2C_ADDRESS_MCP23017_IOExt1)  // Pins 16-31: Steering wheel
  };
};
#endif
#endif // SER_IOEXT2_H
