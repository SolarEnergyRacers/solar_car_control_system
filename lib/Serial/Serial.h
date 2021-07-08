//
// Software Serial (Interconnection to Telemetry / PC)
//

#ifndef SERIAL_H
#define SERIAL_H

#include <SoftwareSerial.h>

void serial_demo_task(void *pvParameter);

class Uart {
private:
public:
    SoftwareSerial serial;
    void init();
    void re_init();
};

#endif // SERIAL_H
