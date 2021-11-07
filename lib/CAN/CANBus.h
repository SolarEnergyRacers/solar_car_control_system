//
// CAN Bus
//

#include "CANRxBuffer.h"
#include "abstract_task.h"

//#ifndef CAN_TX
//#define CAN_TX

void read_can_demo_task(void *pvParameter);

class CANBus : public abstract_task{

  private:
    CANRxBuffer rxBuffer;
    
  public:
    string getName(void);
    void re_init(void);
    void init(void);
    void exit(void);
    void task(void);

    //void create_task(void);

    void onReceive(int packetSize);
    
    SemaphoreHandle_t mutex;
};

//#endif // CAN_H
