#include <Arduino.h>

#include "CANRxBuffer.h"

CANRxBuffer::CANRxBuffer() {
  head = 0;
  tail = 0;
  items = 0;
}

void CANRxBuffer::push(CANPacket packet) {
  rxBuffer[this->head] = packet;
  head++;
  items++;

  if (head >= CAN_RX_BUFFER_SIZE) {
    head = 0;
  }
}

CANPacket CANRxBuffer::pop() {
  CANPacket packet;

  if (items > 0) {
    packet = rxBuffer[tail];

    tail++;
    items--;

    if (tail >= CAN_RX_BUFFER_SIZE) {
      tail = 0;
    }
  } else {
    packet.setID(0);
    packet.setData((uint64_t)0);
  }

  return packet;
}

bool CANRxBuffer::isAvailable() { return (items > 0); }

uint16_t CANRxBuffer::getSize() { return items; }