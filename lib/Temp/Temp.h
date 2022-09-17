//
// Temperature Sensor Array (1-Wire)
//

#ifndef SOLAR_CAR_CONTROL_SYSTEM_TEMP_H
#define SOLAR_CAR_CONTROL_SYSTEM_TEMP_H

#include <DallasTemperature.h> // DS18B20
#include <abstract_task.h>

class Temp : public abstract_task {
private:
  DallasTemperature ds;

public:
  // RTOS task
  string getName() { return "TempSensors"; };
  string init(void);
  string re_init(void);
  void exit(void);
  void task(void);

  void request_temperatures(void);
  int get_num_temp_dev(void);
  float read_tempC_index(int index);
  float read_tempF_index(int index);
};

#endif // SOLAR_CAR_CONTROL_SYSTEM_TEMP_H
