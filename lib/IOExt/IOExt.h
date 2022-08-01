/*
 * MCP23017 I/O Extension over I2C  !!! UNTESTED !!!
 */

#ifndef SER_IOEXT_H
#define SER_IOEXT_H

#include <definitions.h>

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
#define PinDacLifeSign "PinDacLifeSign-p25"
#define PinFwdBwd "FwdBwd"
#define PinGreenLightOut "PinGreenLightOut-p27"
#define PinRelais11 "Relais11"
#define PinIndicatorOutLeft "IndicatorOutLeft-p1"
#define PinIndicatorOutRight "IndicatorOutRight-p2"
#define PinFanOut "FanOut-p28"
#define PinHornOut "HornOut-p3"
#define PinLightOut "LightOut-p5"
#define PinBreakPedal "BreakPedal"
#define PinHeadLightOut "HeadLightOut-p4"
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
#define PinIncrease "Increase"
#define PinConstantModeOff "ConstantModeOff"
#define PinControlMode "PinControlMode"
#define PinPaddleAdjust "PaddleAdjust"
#define PinDecrease "Decrease"
#define PinSdCardDetect "SdCardDetect"
#define PinDUMMY37 "DUMMY37"
#define PinDUMMY38 "DUMMY38"

enum class PinHandleMode { NORMAL, FORCED };

class IOExt : public abstract_task {
public:
  // RTOS task
  string getName(void) { return "IOExt"; };
  string init(void);
  string re_init(void);
  void exit(void);
  void task(void);

  // Class member and functions
  int getPort(int port);
  void setPort(int port, bool value);
  void writeAllPins(PinHandleMode mode = PinHandleMode::NORMAL);
  void readAllPins();
  bool readAndHandlePins(PinHandleMode mode = PinHandleMode::NORMAL);

  static int getIdx(int devNr, int pin) { return devNr * 16 + pin; };
  static int getIdx(int port) { return (port >> 4) * 16 + (port & 0x0F); };

  bool verboseModeDigitalIn = false;
  bool verboseModeDigitalOut = false;

private:
  // bool isInInterruptHandler = false;
  bool isInInputHandler = false;

  void setPortMode(int port, uint8_t mode);
  void handleIoInterrupt();

  MCP23017 IOExtDevs[2] = {
      MCP23017(I2C_ADDRESS_MCP23017_IOExt0), // Pins 00-15: Main board
      MCP23017(I2C_ADDRESS_MCP23017_IOExt1)  // Pins 16-31: Steering wheel
  };
};
#endif // SER_IOEXT_H
