/*
 * PCF8574 I/O Extension over I2C  !!! UNTESTED !!!
 */

#ifndef SER_IOEXT_H
#define SER_IOEXT_H

#include <definitions.h>
#if IOEXT_ON

#include <list>
#include <map>
#include <string>

#include <PCF8574.h>

#include <abstract_task.h>

#include <CarState.h>
#include <CarStatePin.h>
#include <Indicator.h>

#define PinBatOnOff "BatOnOff"
#define PinPvOnOff "PvOnOff"
#define PinMcOnOff "McOnOff"
#define PinEcoPower "EcoPower"
#define PinFwdBwd "FwdBwd"
#define PinDUMMY06 "DUMMY06"
#define PinDUMMY07 "DUMMY07"
#define PinRelais11 "Relais11"
// IOExt1
#define PinIndicatorOutLeft "IndicatorOutLeft"
#define PinIndicatorOutRight "IndicatorOutRight"
#define PinRalais12 "Ralais12"
#define PinRelais31 "Relais31"
#define PinRelais32 "Relais32"
#define PinBreakPedal "BreakPedal"
#define PinDUMMY19 "DUMMY19"
#define PinDUMMY17 "DUMMY17"
// IOExt2
#define PinIndicatorBtnLeft "IndicatorBtnLeft"
#define PinIndicatorBtnRight "IndicatorBtnRight"
#define PinLight "Light"
#define PinHeadLight "HeadLight"
#define PinConstantMode "ConstantMode"
#define PinConstantModeOn "ConstantModeOn"
#define PinHorn "Horn"
#define PinNextScreen "NextScreen"
// IOExt3
#define PinDUMMY31 "DUMMY31"
#define PinReserve1 "Reserve1"
#define PinDUMMY33 "DUMMY33"
#define PinDUMMY34 "DUMMY34"
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
// end pin handler

class IOExt : public abstract_task {
public:

  string getName(void) { return "IOExt"; };

  void init(void);
  void re_init(void);
  void exit(void);
  void task(void);

  void setPort(int port, bool value);
  int getPort(int port);

  void readAll();

  static int getIdx(int devNr, int pin) { return devNr * 8 + pin; };
  static int getIdx(int port) { return (port >> 4) * 8 + (port & 0x0F); };

private:
  static volatile bool ioInterruptRequest;
  bool isInInterruptHandler = false;

  void setPortMode(int port, uint8_t mode);
  void handleIoInterrupt();
  static void ioExt_interrupt_handler() { ioInterruptRequest = true; };

  PCF8574 IOExtDevs[PCF8574_NUM_DEVICES] = {
      PCF8574(I2C_ADDRESS_PCF8574_IOExt0, I2C_SDA, I2C_SCL, I2C_INTERRUPT, ioExt_interrupt_handler),
      PCF8574(I2C_ADDRESS_PCF8574_IOExt1, I2C_SDA, I2C_SCL, I2C_INTERRUPT, ioExt_interrupt_handler),
      PCF8574(I2C_ADDRESS_PCF8574_IOExt2, I2C_SDA, I2C_SCL, I2C_INTERRUPT, ioExt_interrupt_handler),
      PCF8574(I2C_ADDRESS_PCF8574_IOExt3, I2C_SDA, I2C_SCL, I2C_INTERRUPT, ioExt_interrupt_handler)};
};
#endif
#endif // SER_IOEXT_H
