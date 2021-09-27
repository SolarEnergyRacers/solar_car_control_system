/*
 * PCF8574 I/O Extension over I2C  !!! UNTESTED !!!
 */

#ifndef IOEXT_H
#define IOEXT_H

#include <PCF8574.h>
#include <definitions.h>
#include <abstract_task.h>

class IOExt : public abstract_task {
private:

  PCF8574 IOExt[PCF8574_NUM_DEVICES] = {
    PCF8574(I2C_ADDRESS_PCF8574_IOExt0, I2C_SDA, I2C_SCL, I2C_INTERRUPT_PIN_PCF8574, keyPressedInterruptHandler),
    PCF8574(I2C_ADDRESS_PCF8574_IOExt1, I2C_SDA, I2C_SCL, I2C_INTERRUPT_PIN_PCF8574, keyPressedInterruptHandler),
    PCF8574(I2C_ADDRESS_PCF8574_IOExt2, I2C_SDA, I2C_SCL, I2C_INTERRUPT_PIN_PCF8574, keyPressedInterruptHandler),
    PCF8574(I2C_ADDRESS_PCF8574_IOExt3, I2C_SDA, I2C_SCL, I2C_INTERRUPT_PIN_PCF8574, keyPressedInterruptHandler)
  };
  bool isInInterruptHandler = false;
  int taskSleep = 50;
  void handleIoInterrupt(void);

public:

  enum Pin { // high nibble: device number, low nibble: port
    // IOExt0
    DUMMY01 = 0x00, // TODO: add human readeable name
    DUMMY02 = 0x01,
    DUMMY03 = 0x02,
    DUMMY04 = 0x03,
    DUMMY05 = 0x04,
    DUMMY06 = 0x05,
    DUMMY07 = 0x06,
    DUMMY08 = 0x07,
    // IOExt1
    DUMMY11 = 0x10,
    DUMMY12 = 0x11,
    DUMMY13 = 0x12,
    DUMMY14 = 0x13,
    DUMMY15 = 0x14,
    DUMMY16 = 0x15,
    DUMMY17 = 0x16,
    DUMMY18 = 0x17,
    // IOExt2
    DUMMY21 = 0x20,
    DUMMY22 = 0x21,
    DUMMY23 = 0x22,
    DUMMY24 = 0x23,
    DUMMY25 = 0x24,
    DUMMY26 = 0x25,
    DUMMY27 = 0x26,
    DUMMY28 = 0x27,
    // IOExt3
    DUMMY31 = 0x30,
    DUMMY32 = 0x31,
    DUMMY33 = 0x32,
    DUMMY34 = 0x33,
    DUMMY35 = 0x34,
    DUMMY36 = 0x35,
    DUMMY37 = 0x36,
    DUMMY38 = 0x37
  };
  string getName(void) { return string("IOExt"); };
  void init(void);
  void re_init(void);
  void exit(void);
  void task(void);
  void setMode(Pin port, uint8_t mode);
  void set(Pin port, bool value);
  int get(Pin port);

  static void keyPressedInterruptHandler() { ioInterruptRequest = true; };

private:
  static volatile bool ioInterruptRequest;
};

#endif // IOEXT_H
