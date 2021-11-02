//
// SD Card
//

#include <definitions.h>

#include <SD.h> // sd card
#include <SPI.h>

#include <DriverDisplay.h>
#include <SDCard.h>
#include <SPIBus.h>


#define FILENAME "/test.txt"

extern SPIClass SPI;
extern SPIBus spiBus;
extern SDCard sdCard;
extern DriverDisplay driverDisplay;

void SDCard::re_init() { init(); }

void SDCard::init() {
  inited = false;
  char msg[100];

  printf("[?] Init 'SDCard'...\n");
  driverDisplay.print("[?] Init 'SDCard'...\n");

  int count = 0;
  while (!inited && count < 10) {
    count++;

    xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
    // inited = SD.begin(SPI_CS_SDCARD, SPI, 4000000U, "/x");
    inited = SD.begin(SPI_CS_SDCARD, SPI);
    xSemaphoreGive(spiBus.mutex);
  }

  if (!inited) {
    snprintf(msg, 100, "[x] SDCard initialization failed.\n");
    printf(msg);
    driverDisplay.print(msg);
  } else {
    snprintf(msg, 100, "[v] SDCard initialized.\n");
    printf(msg);
    driverDisplay.print(msg);
    snprintf(msg, 100, "   Open file '%s' for append...", FILENAME);
    printf(msg);
    driverDisplay.print(msg);
    // open file
    xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
    dataFile = SD.open(FILENAME, FILE_APPEND); // mode: APPEND: FILE_APPEND, OVERWRITE: FILE_WRITE
    xSemaphoreGive(spiBus.mutex);
    if (dataFile == 0) {
      printf("failed.\n");
    } else {
      printf("ok.\n");
    }
  }
}

void write_sdcard_demo_task(void *pvParameter) {

  // demo counter (written to file)
  int counter = 0;

  while (1) {

    xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
    // check file open
    if (sdCard.isInited() && sdCard.dataFile) {
      // write counter value
      sdCard.dataFile.print(counter);
      sdCard.dataFile.println("");
      debug_printf("[SDCard] Write to sdcard: %d\n", counter++);
    } else {
      // debug_printf("[SDCard] Error opening file.%s", " \n");
    }
    sdCard.dataFile.flush(); // ensure write-back

    xSemaphoreGive(spiBus.mutex);

    // sleep for 1s
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}