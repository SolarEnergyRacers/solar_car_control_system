//
// PCF8574 I/O Extension over I2C  !!! UNTESTED !!!
//

#ifndef SOLAR_CAR_CONTROL_SYSTEM_IOEXT_H
#define SOLAR_CAR_CONTROL_SYSTEM_IOEXT_H

#include <list>
#include <map>
#include <string>

#include <Indicator.h>
#include <PCF8574.h>
#include <abstract_task.h>
#include <definitions.h>

#define BatOnOff "BatOnOff"
#define PvOnOff "PvOnOff"
#define McOnOff "McOnOff"
#define EcoPower "EcoPower"
#define FwdBwd "FwdBwd"
#define DUMMY06 "DUMMY06"
#define DUMMY07 "DUMMY07"
#define Relais11 "Relais11"
// IOExt1
#define Relais21 "Relais21"
#define Relais22 "Relais22"
#define Ralais12 "Ralais12"
#define Relais31 "Relais31"
#define Relais32 "Relais32"
#define BreakPedal "BreakPedal"
#define DUMMY19 "DUMMY19"
#define DUMMY17 "DUMMY17"
// IOExt2
#define IndicatorLeft "IndicatorLeft"
#define IndicatorRight "IndicatorRight"
#define Light "Light"
#define DriveLight "DriveLight"
#define ConstantMode "ConstantMode"
#define ConstantSet "ConstantSet"
#define Horn "Horn"
#define NextScreen "NextScreen"
// IOExt3
#define DUMMY31 "DUMMY31"
#define Reserve1 "Reserve1"
#define DUMMY33 "DUMMY33"
#define DUMMY34 "DUMMY34"
#define DUMMY35 "DUMMY35"
#define DUMMY36 "DUMMY36"
#define DUMMY37 "DUMMY37"
#define DUMMY38 "DUMMY38"

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
void driveLightHandler();
void constantModeHandler();
void constantSetHandler();
// end pin handler

class Pin {
public:
  int port; // high nibble: device number, low nibble: portNr (pin)
  int mode;
  int value;
  int oldValue;
  long timestamp;
  string name;
  void (*handlerFunction)();
};

class IOExt : public abstract_task {
public:
  static void keyPressedInterruptHandler() { ioInterruptRequest = true; };

  string getName(void) { return "IOExt"; };
  void init(void);
  void re_init(void);
  void exit(void);
  void task(void);
  Pin *getPin(int devNr, int pinNr);
  Pin *getPin(int port);
  Pin *getPin(string pinName);
  static Pin pins[IOExtPINCOUNT];
  
private:
  void setMode(int port, uint8_t mode);
  void set(int port, bool value);
  int get(int port);
  void getAll(Pin *pins, int maxCount);
  static int getIdx(int devNr, int pin) { return devNr * 8 + pin; };
  static int getIdx(int port) { return (port >> 4) * 8 + (port & 0x0F); };
  int getIdx(string pinName);
  PCF8574 IOExtDevs[PCF8574_NUM_DEVICES] = {
      PCF8574(I2C_ADDRESS_PCF8574_IOExt0, I2C_SDA, I2C_SCL, I2C_INTERRUPT_PIN_PCF8574, keyPressedInterruptHandler),
      PCF8574(I2C_ADDRESS_PCF8574_IOExt1, I2C_SDA, I2C_SCL, I2C_INTERRUPT_PIN_PCF8574, keyPressedInterruptHandler),
      PCF8574(I2C_ADDRESS_PCF8574_IOExt2, I2C_SDA, I2C_SCL, I2C_INTERRUPT_PIN_PCF8574, keyPressedInterruptHandler),
      PCF8574(I2C_ADDRESS_PCF8574_IOExt3, I2C_SDA, I2C_SCL, I2C_INTERRUPT_PIN_PCF8574, keyPressedInterruptHandler)};
  bool isInInterruptHandler = false;
  int taskSleep = 50;
  std::map<string, int> idxOfPin;
  // std::map<int, Pin> pins; // pins by index
  static volatile bool ioInterruptRequest;
  void handleIoInterrupt();
};
#endif // SOLAR_CAR_CONTROL_SYSTEM_ IOEXT_H
