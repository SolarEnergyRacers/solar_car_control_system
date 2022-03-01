//
// Software Serial (Interconnection to Telemetry / PC)
//

#ifndef SOLAR_CAR_CONTROL_SYSTEM_SERIAL_H
#define SOLAR_CAR_CONTROL_SYSTEM_SERIAL_H

#include <SoftwareSerial.h>

class Uart : public abstract_task {
private:
public:
  // use default Serial2
  // SoftwareSerial Serial2;
  void init();
  void re_init();
  string getName() { return "SerialWire-SerialRadio"; };
  void exit(void);
  void task(void);
};

#endif // SOLAR_CAR_CONTROL_SYSTEM_SERIAL_H
