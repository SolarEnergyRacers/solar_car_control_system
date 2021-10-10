//
// CAN Bus
//
#include <definitions.h>

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#include <Arduino.h>
#include <CAN.h>

#include "CANBus.h"
#include "CANPacket.h"

extern CANBus can;

void CANBus::re_init() { 
  CAN.end();
  CANBus::init(); 
}

void CANBus::init() {
  mutex = xSemaphoreCreateBinary();

  CAN.setPins(CAN_RX, CAN_TX);
  CAN.onReceive(can.onReceive); //ToDo Fix

  if(!CAN.begin(CAN_SPEED)){
    //opening CAN failed :,(
  }

  xSemaphoreGive(mutex);
}


void CANBus::onReceive(int packetSize){

  CANPacket packet;
  uint8_t receivedData[packetSize];

  packet.setID(CAN.packetId());

  for(int i = 0; i < packetSize; i++){
    if(CAN.available()){
      receivedData[i] = (uint8_t) CAN.read();
    }
  }

  packet.setData(receivedData);

  //Check if packet is relevant for board computer
  switch(packet.getID()){
    case BMS_BASE_ADDR | 0xFB:
      //Battery Pack Status
      break;
    case BMS_BASE_ADDR | 0xF1:
      break;
    default:
      break;
  }

}


void read_can_demo_task(void *pvParameter) {
}