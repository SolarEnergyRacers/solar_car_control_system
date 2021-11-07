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

/*void can_task(void *pvParameter){
  can.task();
}*/

string CANBus::getName(){
  return "CAN_Task";
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

void CANBus::exit(){
  // Exit needs to be implemented for Task, here or in abstract_task
  CAN.end();
}

void CANBus::task(){
  CANPacket packet;

  while(1){

    while(this->rxBuffer.isAvailable()){
      packet = this->rxBuffer.pop();

      
      //Do something with packet

      /*if(packet.getID() == 0x700){
        printf("Received Packet:\n");
        printf("id: %02x \n", packet.getID());
        printf("data: %llu \n", packet.getData_ui64());
        printf("data 0: %02x \n", packet.getData_ui32(0));
        printf("data 1: %02x \n\n", packet.getData_ui32(1));
      }*/
    }
    this->sleep(50);
  }
}

void CANBus::onReceive(int packetSize){

  CANPacket packet;
  uint64_t rxData = 0;

  //ToDo implement Semaphore

  packet.setID(CAN.packetId());

  for(int i = 0; i < packetSize; i++){
    if(CAN.available()){
      rxData = rxData | (((uint64_t) CAN.read()) << (i * 8));
    }
  }


  packet.setData(rxData);

  //Add packet to buffer so task can handle it later
  rxBuffer.push(packet);
}