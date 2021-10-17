#include <Arduino.h>

#include "CANRxBuffer.h"

CANRxBuffer::CANRxBuffer(){
    this->head = 0;
    this->tail = 0;
    this->items = 0;
}

void CANRxBuffer::push(CANPacket packet){
    rxBuffer[this->head] = packet;
    this->head++;
    this->items++;

    if(this->head >= CAN_RX_BUFFER_SIZE){
        this->head = 0;
    }
}


CANPacket CANRxBuffer::pop(){
    CANPacket packet;

    if(items > 0){
        packet = rxBuffer[this->tail];

        this->tail++;
        this->items--;

        if(this->tail >= CAN_RX_BUFFER_SIZE){
            this->tail = 0;
        }
    }else{
        packet = *new CANPacket();
    }

    return packet;
}

bool CANRxBuffer::isAvailable(){
    return (this->items > 0);
}

uint16_t CANRxBuffer::getSize(){
    return this->items;
}