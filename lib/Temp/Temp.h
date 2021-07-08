//
// Temperature Sensor Array (1-Wire)
//

#ifndef TEMP_H
#define TEMP_H

#include <DallasTemperature.h> // DS18B20

void read_ds_demo_task(void *pvParameter);


class Temp {
private:
    DallasTemperature ds;
public:
    void init();
    void re_init();
    void request_temperatures(void);
    int get_num_temp_dev(void);
    float read_tempC_index(int index);
    float read_tempF_index(int index);
};

#endif // TEMP_H
