//
// Temperature Sensor Array (1-Wire)
//

#ifndef SOLAR_CAR_CONTROL_SYSTEM_TEMP_H
#define SOLAR_CAR_CONTROL_SYSTEM_TEMP_H

#include <DallasTemperature.h> // DS18B20

void read_ds_demo_task(void *pvParameter);

class Temp { //}: public abstract_task {
private:
  DallasTemperature ds;
  uint32_t sleep_polling_ms;

public:
  void set_SleepTime(uint32_t milliseconds) { sleep_polling_ms = milliseconds; };
  uint32_t get_SleepTime() { return sleep_polling_ms; }

  string init();
  string re_init();
  void request_temperatures(void);
  int get_num_temp_dev(void);
  float read_tempC_index(int index);
  float read_tempF_index(int index);
};

#endif // SOLAR_CAR_CONTROL_SYSTEM_TEMP_H
