//
// CAN Bus
//

#include "CANRxBuffer.h"

//#ifndef CAN_TX
//#define CAN_TX

void read_can_demo_task(void *pvParameter);

class CANBus {
  private:
    CANRxBuffer rxBuffer;
    
  public:
    void re_init();
    void init();

  
    void onReceive(int packetSize);
    
    SemaphoreHandle_t mutex;

};

//#endif // CAN_H
