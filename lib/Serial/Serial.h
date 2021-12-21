//
// Software Serial (Interconnection to Telemetry / PC)
//

#ifndef SOLAR_CAR_CONTROL_SYSTEM_SERIAL_H
#define SOLAR_CAR_CONTROL_SYSTEM_SERIAL_H

#include <SoftwareSerial.h>

void serial_demo_task(void *pvParameter);

class Uart {
private:
public:
  SoftwareSerial serial;
  void init();
  void re_init();
};

#endif // SOLAR_CAR_CONTROL_SYSTEM_SERIAL_H
