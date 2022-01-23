//
// CAN Bus
//

#include <map>

#include "CANRxBuffer.h"
#include "abstract_task.h"

void read_can_demo_task(void *pvParameter);

class CANBus : public abstract_task {

private:
  CANRxBuffer rxBuffer;
  std::map<uint16_t, int32_t> max_ages;
  std::map<uint16_t, int32_t> ages;


public:
  CANBus();
  string getName(void);
  void re_init(void);
  void init(void);
  void exit(void);
  void task(void);

  // void create_task(void);

  void onReceive(int packetSize);

  SemaphoreHandle_t mutex;
};
