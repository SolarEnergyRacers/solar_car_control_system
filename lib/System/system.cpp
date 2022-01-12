//
// System Utility and Helper Functions
//

// standard libraries
#include <fmt/core.h>
#include <fmt/format.h>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <esp_spi_flash.h>
#include <esp_system.h>

#include <system.h>

using namespace std;

void chip_info(void) {

  // print chip information
  esp_chip_info_t chip_info;
  esp_chip_info(&chip_info);
  printf("This is ESP32 chip with %d CPU cores, WiFi%s%s, ", chip_info.cores, (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
         (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

  printf("silicon revision %d, ", chip_info.revision);

  printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
         (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");
}

string memory_info(void) {
  stringstream ss("Not implemented yet.");
  // ss << min_free_8bit_cap;
  // heap_caps_dump_all();
  return ss.str();
}

void restart(void) {

  // count time down
  for (int i = 10; i >= 0; i--) {
    printf("Restarting in %d seconds...\n", i);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }

  // actual restart
  printf("Restarting now.\n");
  fflush(stdout);
  esp_restart();
}
