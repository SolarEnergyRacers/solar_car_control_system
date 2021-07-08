//
// SD Card
//

#include <definitions.h>

#include <SD.h> // sd card

#include <SPIBus.h>

#include "SDCard.h"

#define FILENAME "/test.txt"
File dataFile;

extern SPIBus spiBus;

void init_sdcard(void) {

  // CRITICAL SECTION SPI: start
  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);

  if (!SD.begin(SPI_CS_SDCARD)) {
    printf("[SDCard] Initialization failed\n");
  } else {
    printf("[SDCard] Initialization successful\n");

    // open file
    dataFile = SD.open(FILENAME,
                       FILE_APPEND); // mode: APPEND: FILE_APPEND, OVERWRITE: FILE_WRITE
  }

  xSemaphoreGive(spiBus.mutex);
  // CRITICAL SECTION SPI: end
}

void write_sdcard_demo_task(void *pvParameter) {

  // demo counter (written to file)
  int counter = 0;

  while (1) {

    // CRITICAL SECTION SPI: start
    xSemaphoreTake(spiBus.mutex, portMAX_DELAY);

    // check file open
    if (dataFile) {
      // write counter value
      dataFile.print(counter);
      dataFile.println("");
      printf("[SDCard] Write to sdcard: %d\n", counter++);
    } else {
      printf("[SDCard] Error opening file.\n");
    }
    dataFile.flush(); // ensure write-back

    xSemaphoreGive(spiBus.mutex);
    // CRITICAL SECTION SPI: end

    // sleep for 1s
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}