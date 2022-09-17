#include <Arduino.h>

#include "CANPacket.h"

CANPacket::CANPacket() {
  id = 0;
  data.data_u64 = 0;
}

CANPacket::CANPacket(uint16_t id, uint8_t data[]) {
  this->id = id;
  for (int i = 0; i < 8; i++) {
    this->data.data_u8[i] = data[i];
  }
}

CANPacket::CANPacket(uint16_t id, uint64_t data) {
  this->id = id;
  this->data.data_u64 = data;
}

void CANPacket::setID(uint16_t id) { this->id = id; }

uint16_t CANPacket::getID() { return id; }

void CANPacket::setData(uint8_t data[]) {
  for (int i = 0; i < 8; i++) {
    this->data.data_u8[i] = data[i];
  }
}

void CANPacket::setData(uint64_t data) { this->data.data_u64 = data; }

uint64_t CANPacket::getData_ui64() { return this->data.data_u64; }

int64_t CANPacket::getData_i64() { return this->data.data_64; }

uint32_t CANPacket::getData_ui32(uint8_t index) { return this->data.data_u32[index]; }

int32_t CANPacket::getData_i32(uint8_t index) { return this->data.data_32[index]; }

uint16_t CANPacket::getData_ui16(uint8_t index) { return this->data.data_u16[index]; }

int16_t CANPacket::getData_i16(uint8_t index) { return this->data.data_16[index]; }

uint8_t CANPacket::getData_ui8(uint8_t index) { return this->data.data_u8[index]; }

int8_t CANPacket::getData_i8(uint8_t index) { return this->data.data_8[index]; }

bool CANPacket::getData_b(uint8_t index) { return ((this->data.data_u64 >> index) & 0x0000000000000001); }

float CANPacket::getData_f32(uint8_t index) { return this->data.data_fp[index]; }