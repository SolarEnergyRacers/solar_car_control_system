//
// Software Serial (Interconnection to Telemetry / PC)
//

#ifndef SOLAR_CAR_CONTROL_SYSTEM_SERIAL_H
#define SOLAR_CAR_CONTROL_SYSTEM_SERIAL_H

#include <SoftwareSerial.h>

class Uart : public abstract_task {
private:
  uint32_t sleep_polling_ms = 400;

public:
  // RTOS task
  void set_SleepTime(uint32_t milliseconds) { sleep_polling_ms = milliseconds; };
  uint32_t get_SleepTime() { return sleep_polling_ms; };
  string getName() { return "SerialWire-SerialRadio"; };
  void init(void);
  void re_init(void);
  void exit(void);
  void task(void);

  // Class functions and members
};

#endif // SOLAR_CAR_CONTROL_SYSTEM_SERIAL_H
