//
// CAN Bus
//

#ifndef SOLAR_CAR_CONTROL_SYSTEM_CANBUS_H
#define SOLAR_CAR_CONTROL_SYSTEM_CANBUS_H

#include <abstract_task.h>
#include <definitions.h>
class CanBus : public abstract_task {
private:
  uint64_t bus_freq = 125E3; // 125kbit/s
public:
  SemaphoreHandle_t mutex;
  string getName(void);
  void init();
  void re_init();
  void read_poll();
  void write(uint64_t address, char* data, uint64_t length);
  void write_extended(uint64_t address, char* data, uint64_t length);
  void exit();
  void task();
};

#endif // SOLAR_CAR_CONTROL_SYSTEM_CANBUS_H
