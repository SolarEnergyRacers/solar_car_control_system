//
// Software Serial (Interconnection to Telemetry / PC)
//

#ifndef SOLAR_CAR_CONTROL_SYSTEM_SERIAL_H
#define SOLAR_CAR_CONTROL_SYSTEM_SERIAL_H

#include <SoftwareSerial.h>
#include <abstract_task.h>

class Uart : public abstract_task {
public:
  // RTOS task
  string getName() { return "SerialWire-SerialRadio"; };
  string init(void);
  string re_init(void);
  void exit(void);
  void task(void);
};

#endif // SOLAR_CAR_CONTROL_SYSTEM_SERIAL_H
