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

      printf("----------------\n");
      printf("id: %X\n", packet.getID());
      printf("data 32 0: %X\n", packet.getData_ui32(0));
      printf("data 32 1: %X\n", packet.getData_ui32(1));

      //Do something with packet
      switch(packet.getID()){
        case BMS_BASE_ADDR:
          // heartbeat packet.getData_ui32(0)
          break;
        case BMS_BASE_ADDR | 0xFA:
          // Battery Voltage mV packet.getData_ui32(0)
          // Battery Current mA packet.getData_i32(1)
          break;
        case BMS_BASE_ADDR | 0xF8:
          // Battery min Cell Voltage mV packet.getData_ui16(0)
          // Battery max Cell Voltage mV packet.getData_ui16(1)
          // CMU number with min Cell Voltage packet.getData_ui8(4)
          // Cell number with min Voltage packet.getData_ui8(5)
          // CMU number with max Cell Voltage packet.getData_ui8(6)
          // Cell number with max Voltage packet.getData_ui8(7)
          break;
        case BMS_BASE_ADDR | 0xF7:
          // Contactor 1 driver error packet.getData_b(0)
          // Contactor 2 driver error packet.getData_b(1)
          // Contactor 1 driver output status packet.getData_b(2)
          // Contactor 2 driver output status packet.getData_b(3)

          // 12V contactor supply OK packet.getData_b(4)
          // Contactor 3 driver error packet.getData_b(5)
          // Contactor 3 driver output status packet.getData_b(6)

          // Precharge State packet.getData_ui8(1)
          /*
              0=Error
              1=Idle
              2=Measure
              3=Precharge
              4=Run
              5=Enable Pack
          */

          // Precharge Timer info also available
          break;
        case BMS_BASE_ADDR | 0xFD:
          // Cell over voltage packet.getData_b(0)
          // Cell under voltage packet.getData_b(1)
          // Cell over Temp packet.getData_b(2)
          // Measurement untrusted packet.getData_b(3)

          // CMU Comm Timeout packet.getData_b(4)
          // Vehicle Comm Timeout packet.getData_b(5)
          // BMU in Setup Mode packet.getData_b(6)
          // CMU CAN Power Status packet.getData_b(7)

          // Pack isolation test fail packet.getData_b(8)
          // SOC measurement invalid packet.getData_b(9)
          // CAN 12V in low, about to shutdown packet.getData_b(10)
          // Contactor Stuck/Not engaged packet.getData_b(11)

          // Extra Cell present packet.getData_b(12)
          break;
        case MPPT1_BASE_ADDR | 0x1:
          // MPPT1 Output Voltage V packet.getData_f32(0)
          // MPPT1 Output Current A packet.getData_f32(1)
          break;
        case MPPT2_BASE_ADDR | 0x1:
          // MPPT2 Output Voltage V packet.getData_f32(0)
          // MPPT2 Output Current A packet.getData_f32(1)
          break;
        case MPPT3_BASE_ADDR | 0x1:
          // MPPT3 Output Voltage V packet.getData_f32(0)
          // MPPT3 Output Current A packet.getData_f32(1)
          break;
      }
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