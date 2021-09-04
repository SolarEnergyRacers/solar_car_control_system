//
// PCF8574 I/O Extension over I2C  !!! UNTESTED !!!
//

#ifndef SOLAR_CAR_CONTROL_SYSTEM_IOEXT_H
#define SOLAR_CAR_CONTROL_SYSTEM_IOEXT_H

#include <PCF8574.h>
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

public:
  string getName(void) { return "IOExt"; };
  void init(void);
  void re_init(void);
  void exit(void);
  void task(void);
  void set_ioext(int port, bool value);
  int get_ioext(int port);

  static void keyPressedInterruptHandler() { ioInterruptRequest = true; };

private:
  static volatile bool ioInterruptRequest;
};

#endif //SOLAR_CAR_CONTROL_SYSTEM_ IOEXT_H
