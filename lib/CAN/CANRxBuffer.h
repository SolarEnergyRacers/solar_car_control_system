#include <Arduino.h>
#include <vector>

#include "CANPacket.h"
#include "definitions.h"

class CANRxBuffer {
private:
  uint8_t head;
  uint8_t tail;
  uint16_t items;
  CANPacket rxBuffer[CAN_RX_BUFFER_SIZE];

public:
  CANRxBuffer();

  void push(CANPacket packet);
  CANPacket pop();
  bool isAvailable();
  uint16_t getSize();
};