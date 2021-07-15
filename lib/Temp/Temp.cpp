//
// Temperature Sensor Array (1-Wire)
//

#include <OneWireBus.h>

#include "Temp.h"

#include <DallasTemperature.h> // DS18B20

extern OneWireBus oneWireBus;

void Temp::re_init() { init(); }

void Temp::init(void) {

  // CRITICAL SECTION ONEWIRE: start
  xSemaphoreTake(oneWireBus.mutex, portMAX_DELAY);

  // init devices
  ds = DallasTemperature(&oneWireBus.oneWire);
  ds.begin();

  // report devices found
  printf("[OneWire] num devices on bus: %d\n", ds.getDeviceCount());

  // clear old search
  oneWireBus.oneWire.reset_search();

  // report details of all devices (address, resolution, power mode, ..)
  DeviceAddress device_addr;
  while (oneWireBus.oneWire.search(device_addr)) {

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

  xSemaphoreGive(oneWireBus.mutex);
  // CRITICAL SECTION ONEWIRE: end
}

void Temp::request_temperatures(void) {

  // CRITICAL SECTION ONEWIRE: start
  xSemaphoreTake(oneWireBus.mutex, portMAX_DELAY);

  // request all temperature sensor readings
  ds.requestTemperatures();

  xSemaphoreGive(oneWireBus.mutex);
  // CRITICAL SECTION ONEWIRE: end
}

int Temp::get_num_temp_dev(void) {
  return ds.getDeviceCount(); // no mutex required, since it does not actually
                              // communicate with the sensor
}

float Temp::read_tempC_index(int index) {

  // check bound
  if (index < 0 || index >= get_num_temp_dev()) {
    return NAN;
  }

  return ds.getTempCByIndex(index); // no mutex required, since it does not
                                    // actually communicate with the sensor
}

float Temp::read_tempF_index(int index) {

  // check bound
  if (index < 0 || index >= get_num_temp_dev()) {
    return NAN;
  }

  return ds.getTempFByIndex(index); // no mutex required, since it does not
                                    // actually communicate with the sensor
}

extern Temp ds;
void read_ds_demo_task(void *pvParameter) {

  // polling loop
  while (1) {

    // request all temperature sensor readings
    ds.request_temperatures();

    // print previously fetched results
    for (int i = 0; i < ds.get_num_temp_dev(); i++) {
      printf("[DS18B20] Temperature: %fC / %fF\n", ds.read_tempC_index(i), ds.read_tempF_index(i));
    }

    // sleep for 1s
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
