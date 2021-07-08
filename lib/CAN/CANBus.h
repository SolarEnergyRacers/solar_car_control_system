//
// CAN Bus
//

#ifndef CAN_H
#define CAN_H

#include <CAN_config.h>
#include <ESP32CAN.h>

void read_can_demo_task(void *pvParameter);

class CanBus {
private:
public:
    SemaphoreHandle_t mutex;
    CAN_device_t cfg;
    void init();
    void re_init();
};

#endif // CAN_H
