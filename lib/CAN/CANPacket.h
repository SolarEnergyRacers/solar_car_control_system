#include <Arduino.h>

typedef union {
  float data_fp[2];
  uint8_t data_u8[8];
  int8_t data_8[8];
  uint16_t data_u16[4];
  int16_t data_16[4];
  uint32_t data_u32[2];
  int32_t data_32[2];
  uint64_t data_u64;
  int64_t data_64;
} group_64;

class CANPacket {
private:
  uint16_t id;
  group_64 data;

public:
  CANPacket();
  CANPacket(uint16_t id, uint8_t data[]);
  CANPacket(uint16_t id, uint64_t data);

  CANPacket &operator=(const CANPacket &that) {
    if (this != &that) {
      this->id = that.id;
      this->data = that.data;
    }

    return *this;
  };

  void setID(uint16_t id);
  uint16_t getID();

  void setData(uint8_t data[]);
  void setData(uint64_t data);

  uint64_t getData_ui64();
  int64_t getData_i64();

  uint32_t getData_ui32(uint8_t index);
  int32_t getData_i32(uint8_t index);

  uint16_t getData_ui16(uint8_t index);
  int16_t getData_i16(uint8_t index);

  uint8_t getData_ui8(uint8_t index);
  int8_t getData_i8(uint8_t index);

  bool getData_b(uint8_t index);

  float getData_f32(uint8_t index);
};