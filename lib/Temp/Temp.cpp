//
// Temperature Sensor Array (1-Wire)
//

#include <definitions.h>

// standard libraries
#include <fmt/core.h>
#include <iostream>
#include <stdio.h>
#include <string>

#include <Console.h>
#include <DallasTemperature.h> // DS18B20
#include <OneWireBus.h>
#include <Temp.h>

extern OneWireBus oneWireBus;
extern Console console;
extern Temp temp;

string Temp::re_init() { return init(); }

string Temp::init(void) {
  bool hasError = false;
  console << "[  ] Init 'Temp (OneWire)'...\n";
  xSemaphoreTake(oneWireBus.mutex, portMAX_DELAY);

  // init devices
  ds = DallasTemperature(&oneWireBus.oneWire);
  ds.begin();

  // report devices found
  console << fmt::format("     OneWire devices on bus: {}\n", ds.getDeviceCount());

  // clear old search
  oneWireBus.oneWire.reset_search();

  // report details of all devices (address, resolution, power mode, ..)
  DeviceAddress device_addr;
  while (oneWireBus.oneWire.search(device_addr)) {

    console << "     [DS18B20] Address:";
    for (uint8_t i = 0; i < 8; i++) {

      console << fmt::format(" {}", device_addr[i]);
    }
    console << " ";

    console << fmt::format("     Resolution: {}", ds.getResolution(device_addr));
    console << " ";

    console << "    Power Mode: ";
    if (ds.isParasitePowerMode()) {
      console << "External\n";
    } else {
      console << "Parasite\n";
    }
  }

  xSemaphoreGive(oneWireBus.mutex);
  return fmt::format("[{}] Temp initialized.", hasError ? "--" : "ok");
}

void Temp::exit() {}

void Temp::request_temperatures(void) {

  xSemaphoreTake(oneWireBus.mutex, portMAX_DELAY);

  // request all temperature sensor readings
  ds.requestTemperatures();

  xSemaphoreGive(oneWireBus.mutex);
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

void Temp::task() {
  // polling loop
  while (1) {

    // request all temperature sensor readings
    request_temperatures();

    // print previously fetched results
    for (int i = 0; i < get_num_temp_dev(); i++) {
      console << fmt::format("    [DS18B20] Temperature: {:4.2f}C / {:4.2f}F\n", read_tempC_index(i), read_tempF_index(i));
    }

    vTaskDelay(sleep_polling_ms / portTICK_PERIOD_MS);
  }
}
