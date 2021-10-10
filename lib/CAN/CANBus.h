//
// CAN Bus
//
#include "CANPacket.h"

//#ifndef CAN_TX
//#define CAN_TX

void read_can_demo_task(void *pvParameter);

class CANBus {
  private:
    CANPacket rxBuffer[CAN_RX_BUFFER_SIZE];
    
  public:
    void re_init();
    void init();

    void onReceive(int packetSize);
    
    SemaphoreHandle_t mutex;

};

//#endif // CAN_H
