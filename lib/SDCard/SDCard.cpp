//
// SD Card
//

#include <definitions.h>

#include <SD.h> // sd card

#include <SPIBus.h>

#include <SDCard.h>

#define FILENAME "/test.txt"

extern SPIBus spiBus;
extern SDCard sdCard;

void SDCard::re_init() { init(); }

void SDCard::init() {

  // CRITICAL SECTION SPI: start
  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);

  if (!SD.begin(SPI_CS_SDCARD)) {
    printf("[SDCard] Initialization failed\n");
  } else {
    printf("[SDCard] Initialization successful, open file '%s' for append\n", FILENAME);

    // open file
    dataFile = SD.open(FILENAME, FILE_APPEND); // mode: APPEND: FILE_APPEND, OVERWRITE: FILE_WRITE
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
    if (sdCard.dataFile) {
      // write counter value
      sdCard.dataFile.print(counter);
      sdCard.dataFile.println("");
      printf("[SDCard] Write to sdcard: %d\n", counter++);
    } else {
      printf("[SDCard] Error opening file.\n");
    }
    sdCard.dataFile.flush(); // ensure write-back

    xSemaphoreGive(spiBus.mutex);
    // CRITICAL SECTION SPI: end

    // sleep for 1s
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}