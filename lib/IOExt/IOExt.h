//
// PCF8574 I/O Extension over I2C  !!! UNTESTED !!!
//

#ifndef IOEXT_H
#define IOEXT_H

#include <PCF8574.h> // PCF8574
#include <definitions.h>

class IOExt : public abstract_task {
private:
  // TODO: is there an IOExt1? If yes: we should integrate this here too, see ADC multi device as a reference, Else: remove '2'
  PCF8574 IOExt2 = PCF8574(I2C_ADDRESS_PCF8574_IOExt2, I2C_SDA, I2C_SCL, I2C_INTERRUPT_PIN_PCF8574, keyPressedInterruptHandler);
  bool isInInterruptHandler = false;
  // simulation - start (for simulation purpose)
  int speed = 0;
  int acceleration = 0;
  // simulation - end
  int taskSleep = 50;
  void handleIoInterrupt(void);
  void speedCheck(int speed);

public:
  string getName(void) { return "IOExt"; };
  void init(void);
  void re_init(void);
  void exit(void);
  void task(void);
  void set_ioext(int port, bool value);
  int get_ioext(int port);

  static volatile bool ioInterruptRequest;
  static void keyPressedInterruptHandler() { ioInterruptRequest = true; };
};

#endif // IOEXT_H
