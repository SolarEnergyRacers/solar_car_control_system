#include <Arduino.h>

#include "CANPacket.h"

CANPacket::CANPacket(){
    id = 0;
    data = 0;
}

CANPacket::CANPacket(uint16_t id, uint8_t data[]){
    this->id = id;
    
    for(uint8_t i = 0; i < sizeof(data); i++){
        this->data = this->data + (data[i] << (i * 8));
    }
}

CANPacket::CANPacket(uint16_t id, uint64_t data){
    this->id = id;
    this->data = data;
}

void CANPacket::setID(uint16_t id){
    this->id = id;
}

uint16_t CANPacket::getID(){
    return this->id;
}

void CANPacket::setData(uint8_t data[]){
    for(uint8_t i = 0; i < sizeof(data); i++){
        this->data = this->data + (data[i] << (i * 8));
    }
}

void CANPacket::setData(uint64_t data){
    this->data = data;
}

uint64_t CANPacket::getData_ui64(){
    return this->data;
}

//ToDo Test unsigned->signed on ESP32 runtime
int64_t CANPacket::getData_i64(){
    int64_t result = this->data & 0xFFFFFFFFFFFFFFFF;
    return result;
}

uint32_t CANPacket::getData_ui32(uint8_t index){
    return (uint32_t) (this->data >> (32*index)) & 0x00000000FFFFFFFF;
}

int32_t CANPacket::getData_i32(uint8_t index){
    return (int32_t) (this->data >> (32*index)) & 0x00000000FFFFFFFF;
}

uint16_t CANPacket::getData_ui16(uint8_t index){
    return (uint16_t) (this->data >> (16*index)) & 0x000000000000FFFF;
}

int16_t CANPacket::getData_i16(uint8_t index){
    return (int16_t) (this->data >> (16*index)) & 0x000000000000FFFF;
}

uint8_t CANPacket::getData_ui8(uint8_t index){
    return (uint8_t) (this->data >> (8*index)) & 0x00000000000000FF;
}

int8_t CANPacket::getData_i8(uint8_t index){
    return (int8_t) (this->data >> (8*index)) & 0x00000000000000FF;
}

float CANPacket::getData_f32(uint8_t index){
    uint32_t ival = (uint32_t) (this->data >> (32*index)) & 0x00000000FFFFFFFF;
    float fval = 0.0;
    
    memcpy(&fval, &ival, sizeof(fval));

    return fval;
}