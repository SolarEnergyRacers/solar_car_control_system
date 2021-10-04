//
// PCF8574 I/O Extension over I2C  !!! UNTESTED !!!
//

#ifndef SOLAR_CAR_CONTROL_SYSTEM_IOEXT_H
#define SOLAR_CAR_CONTROL_SYSTEM_IOEXT_H

#include <PCF8574.h>
#include <abstract_task.h>
#include <definitions.h>
#include <Indicator.h>

#include <map>
#include <string>
#include <list>

class Pin {
public:
  int port;
  int mode;
  int value;
  string name;
  void (*handlerFunction)();
};

//void (*handlerFunction[32])(int);

// known pin handler
void batteryOnOffHandler();
void mcOnOffHandler();
void pvOnOffHandler();
void indicatorHandler();
// end pin handler

class IOExt : public abstract_task {
private:
  PCF8574 IOExt[PCF8574_NUM_DEVICES] = {
      PCF8574(I2C_ADDRESS_PCF8574_IOExt0, I2C_SDA, I2C_SCL, I2C_INTERRUPT_PIN_PCF8574, keyPressedInterruptHandler),
      PCF8574(I2C_ADDRESS_PCF8574_IOExt1, I2C_SDA, I2C_SCL, I2C_INTERRUPT_PIN_PCF8574, keyPressedInterruptHandler),
      PCF8574(I2C_ADDRESS_PCF8574_IOExt2, I2C_SDA, I2C_SCL, I2C_INTERRUPT_PIN_PCF8574, keyPressedInterruptHandler),
      PCF8574(I2C_ADDRESS_PCF8574_IOExt3, I2C_SDA, I2C_SCL, I2C_INTERRUPT_PIN_PCF8574, keyPressedInterruptHandler)};
  bool isInInterruptHandler = false;
  int taskSleep = 50;
  void handleIoInterrupt();

public:
  static void keyPressedInterruptHandler() { ioInterruptRequest = true; };
  //static list<void (*)()> pinHandlerList;
  // high nibble: device number, low nibble: port
  // TODO: add human readeable name

  Pin pins[IOExtPINCOUNT] = {{0x00, INPUT_PULLUP, 0, "BatOnOff", batteryOnOffHandler},
                             {0x01, INPUT_PULLUP, 0, "PvOnOff", pvOnOffHandler},
                             {0x02, INPUT_PULLUP, 0, "McOnOff", mcOnOffHandler},
                             {0x03, INPUT_PULLUP, 0, "EcoPower", NULL},
                             {0x04, INPUT_PULLUP, 0, "FwdBwd", NULL},
                             {0x05, INPUT_PULLUP, 0, "DUMMY06", NULL},
                             {0x06, INPUT_PULLUP, 0, "unused", NULL},
                             {0x07, OUTPUT, 0, "Relais11", NULL},
                             // IOExt1
                             {0x10, OUTPUT, 0, "Relais21", NULL},
                             {0x11, OUTPUT, 0, "Relais22", NULL},
                             {0x12, OUTPUT, 0, "Ralais12", NULL},
                             {0x13, OUTPUT, 0, "Relais31", NULL},
                             {0x14, OUTPUT, 0, "Relais32", NULL},
                             {0x15, INPUT_PULLUP, 0, "BreakPedal", NULL},
                             {0x16, INPUT_PULLUP, 0, "unused", NULL},
                             {0x17, INPUT_PULLUP, 0, "unused", NULL},
                             // IOExt2
                             {0x20, INPUT_PULLUP, 0, "IndicatorLeft", indicatorHandler},
                             {0x21, INPUT_PULLUP, 0, "IndicatorRight", indicatorHandler},
                             {0x22, INPUT_PULLUP, 0, "DUMMY23", NULL},
                             {0x23, INPUT_PULLUP, 0, "DUMMY24", NULL},
                             {0x24, INPUT_PULLUP, 0, "DUMMY25", NULL},
                             {0x25, INPUT_PULLUP, 0, "DUMMY26", NULL},
                             {0x26, INPUT_PULLUP, 0, "DUMMY27", NULL},
                             {0x27, INPUT_PULLUP, 0, "DUMMY28", NULL},
                             // IOExt3
                             {0x30, INPUT_PULLUP, 0, "DUMMY31", NULL},
                             {0x31, INPUT_PULLUP, 0, "DUMMY32", NULL},
                             {0x32, INPUT_PULLUP, 0, "DUMMY33", NULL},
                             {0x33, INPUT_PULLUP, 0, "DUMMY34", NULL},
                             {0x34, INPUT_PULLUP, 0, "DUMMY35", NULL},
                             {0x35, INPUT_PULLUP, 0, "DUMMY36", NULL},
                             {0x36, INPUT_PULLUP, 0, "DUMMY37", NULL},
                             {0x37, INPUT_PULLUP, 0, "DUMMY38", NULL}};

  string getName(void) { return string("IOExt"); };
  void init(void);
  void re_init(void);
  void exit(void);
  void task(void);
  void setMode(int port, uint8_t mode);
  void set(int port, bool value);
  int get(int port);
  void getAll(Pin *pins, int maxCount);
  int getIdx(int port);
  
private:
  static volatile bool ioInterruptRequest;
};
#endif // SOLAR_CAR_CONTROL_SYSTEM_ IOEXT_H
