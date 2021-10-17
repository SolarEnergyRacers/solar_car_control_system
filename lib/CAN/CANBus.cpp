//
// CAN Bus
//
#include <definitions.h>

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#include <Arduino.h>
#include <CAN.h>

#include "CANBus.h"

extern CANBus can;

void onReceiveForwarder(int packetSize){
  can.onReceive(packetSize);
}

void CANBus::re_init() { 
  CAN.end();
  CANBus::init(); 
}

void CANBus::init() {
  mutex = xSemaphoreCreateBinary();

  CAN.setPins(CAN_RX, CAN_TX);
  CAN.onReceive(onReceiveForwarder); //not sure if works

  if(!CAN.begin(CAN_SPEED)){
    //opening CAN failed :,(
  }

  xSemaphoreGive(mutex);
}


void CANBus::onReceive(int packetSize){

  CANPacket packet;
  uint64_t rxData = 0;

  packet.setID(CAN.packetId());

  for(int i = 0; i < packetSize; i++){
    if(CAN.available()){
      rxData = rxData | (((uint8_t) CAN.read()) << (i * 8));
    }
  }

  packet.setData(rxData);

  //Add packet to buffer so task can handle it later
  rxBuffer.push(packet);
}



void read_can_demo_task(void *pvParameter) {
  /*  //Check if packet is relevant for board computer
  switch(packet.getID()){
    case BMS_BASE_ADDR | 0xFB:
      //Battery Pack Status
      break;
    case BMS_BASE_ADDR | 0xF1:
      break;
    default:
      break;
  }*/
}