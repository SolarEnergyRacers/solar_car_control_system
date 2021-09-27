//
// CAN Bus
//
#include <Arduino.h>

#include <definitions.h>

#include <abstract_task.h>
#include <definitions.h>

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#include <CAN.h>

#include "CANBus.h"

/*
 * Battery Management System  CANIDs:
 * BASE = 0x700 (default: 0x600)
 * +0x0:       BMU heartbeat & serial number
 * +0x01-0xEF: CMU status, temperature and voltage telemetry (TODO: configure!)
 * +0xF4:      Pack SOC
 * +0xF5:      Balance SOC
 * +0xF6:      Charger Control Information
 * +0xF7:      Precharge Status, 12V Status
 * +0xF8:      Min/Max Cell Voltage
 * +0xF9:      Min/Max Cell Temperature
 * +0xFA:      Battery Pack Voltage & Current
 * +0xFB:      Battery Pack Status
 * +0xFC:      Fan & 12V Supply Status
 * +0xFD:      Extended Battery Pack Status
 * 
 * MPPT IDs: TODO
 * 
 * 
 * !!! BUG: we have to comment out the line: SPI.usingInterrupt(digitalPinToInterrupt(_intPin)); in MCP2515.cpp !!!
 * 
 */

void CanBus::re_init() { CanBus::init(); }

void CanBus::exit(){
  // TODO
}

string CanBus::getName(void){
  return std::string("CanBus");
}

void CanBus::init() {

  mutex = xSemaphoreCreateBinary();

  CAN.setClockFrequency(8e6);

  // TODO: CAN.setPins(int cs, int irq);

  // start the CAN bus at 125 kbps
  if(CAN.begin(bus_freq)) {
      Serial.println("CAN initialization succeeded..");
  } else {
      Serial.println("CAN initialization failed.");
  }

  xSemaphoreGive(mutex);
}

void CanBus::read_poll(){

  // try to parse packet
  int packetSize = CAN.parsePacket();

  if (packetSize) {
    // received a packet
    Serial.print("Received ");

    if (CAN.packetExtended()) {
      Serial.print("extended ");
    }

    if (CAN.packetRtr()) {
      // Remote transmission request, packet contains no data
      Serial.print("RTR ");
    }

    Serial.print("packet with id 0x");
    Serial.print(CAN.packetId(), HEX);

    if (CAN.packetRtr()) {
      Serial.print(" and requested length ");
      Serial.println(CAN.packetDlc());
    } else {
      Serial.print(" and length ");
      Serial.println(packetSize);

      // only print packet data for non-RTR packets
      while (CAN.available()) {
        Serial.print((char)CAN.read());
      }
      Serial.println();
    }

    Serial.println();
  }
}

void CanBus::write(uint64_t address, char* data, uint64_t length){

  // send packet: id is 11 bits, packet can contain up to 8 bytes of data
  CAN.beginPacket(address);
  for(size_t i = 0; i < length; i++){
    CAN.write(data[i]);
  }
  CAN.endPacket();
}

void CanBus::write_extended(uint64_t address, char* data, uint64_t length){

  // send extended packet: id is 29 bits, packet can contain up to 8 bytes of data
  CAN.beginExtendedPacket(address);
  for(size_t i = 0; i < length; i++){
    CAN.write(data[i]);
  }
  CAN.endPacket();
}

void CanBus::task() {

  while (1) {
    Serial.println("CAN While");
    xSemaphoreTake(mutex, portMAX_DELAY);

    read_poll();

    char hello[10] = { "hello" };
    write(0x42, hello, strnlen(hello, 10));

    xSemaphoreGive(mutex);

    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}
