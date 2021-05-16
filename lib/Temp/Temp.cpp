//
// Temperature Sensor Array (1-Wire)
//

#include <OneWireBus.h>

#include "Temp.h"

#include <DallasTemperature.h> // DS18B20
DallasTemperature ds(&oneWire);

void init_ds(void) {

  // CRITICAL SECTION ONEWIRE: start
  xSemaphoreTake(onewire_mutex, portMAX_DELAY);

  // init devices
  ds.begin();

  // report devices found
  printf("[OneWire] num devices on bus: %d\n", ds.getDeviceCount());

  // clear old search
  oneWire.reset_search();

  // report details of all devices (address, resolution, power mode, ..)
  DeviceAddress device_addr;
  while (oneWire.search(device_addr)) {

    printf("[DS18B20] Address:");
    for (uint8_t i = 0; i < 8; i++) {

      printf(" %d", device_addr[i]);
    }
    printf(" ");

    printf("Resolution: %d", ds.getResolution(device_addr));
    printf(" ");

    printf("Power Mode: ");
    if (ds.isParasitePowerMode()) {
      printf("External");
    } else {
      printf("Parasite");
    }
    printf("\n");
  }

  xSemaphoreGive(onewire_mutex);
  // CRITICAL SECTION ONEWIRE: end
}

void request_temperatures(void) {

  // CRITICAL SECTION ONEWIRE: start
  xSemaphoreTake(onewire_mutex, portMAX_DELAY);

  // request all temperature sensor readings
  ds.requestTemperatures();

  xSemaphoreGive(onewire_mutex);
  // CRITICAL SECTION ONEWIRE: end
}

int get_num_temp_dev(void) {
  return ds.getDeviceCount(); // no mutex required, since it does not actually
                              // communicate with the sensor
}

float read_tempC_index(int index) {

  // check bound
  if (index < 0 || index >= get_num_temp_dev()) {
    return NAN;
  }

  return ds.getTempCByIndex(index); // no mutex required, since it does not
                                    // actually communicate with the sensor
}

float read_tempF_index(int index) {

  // check bound
  if (index < 0 || index >= get_num_temp_dev()) {
    return NAN;
  }

  return ds.getTempFByIndex(index); // no mutex required, since it does not
                                    // actually communicate with the sensor
}

void read_ds_demo_task(void *pvParameter) {

  // polling loop
  while (1) {

    // request all temperature sensor readings
    request_temperatures();

    // print previously fetched results
    for (int i = 0; i < get_num_temp_dev(); i++) {
      printf("[DS18B20] Temperature: %fC / %fF\n", read_tempC_index(i),
             read_tempF_index(i));
    }

    // sleep for 1s
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
