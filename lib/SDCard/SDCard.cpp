//
// SD Card
//

#include <definitions.h>

// standard libraries
#include <fmt/core.h>
#include <iostream>
#include <stdio.h>
#include <string>

#include <SD.h> // sd card
#include <SPI.h>

#include <DriverDisplay.h>
#include <Helper.h>
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

  string s = "[?] Init 'SDCard'...\n";
  cout << s;
  driverDisplay.print(s.c_str());

  int count = 0;
  while (!inited && count < 10) {
    count++;

    xSemaphoreTakeT(spiBus.mutex);
    // inited = SD.begin(SPI_CS_SDCARD, SPI, 4000000U, "/x");
    inited = SD.begin(SPI_CS_SDCARD, SPI);
    xSemaphoreGive(spiBus.mutex);
  }

  if (!inited) {
    s = "[x] SDCard initialization failed.\n";
    cout << s;
    driverDisplay.print(s.c_str());
  } else {
    s = "[v] SDCard initialized.\n";
    cout << s;
    driverDisplay.print(s.c_str());
    s = fmt::format("   Open file '{}' for append...", FILENAME);
    cout << s;
    driverDisplay.print(s.c_str());

    xSemaphoreTakeT(spiBus.mutex);
    // open file
    dataFile = SD.open(FILENAME, FILE_APPEND); // mode: APPEND: FILE_APPEND, OVERWRITE: FILE_WRITE
    xSemaphoreGive(spiBus.mutex);

    if (dataFile == 0) {
      cout << "failed." << endl;
    } else {
      cout << "ok." << endl;
    }
  }
}

void write_sdcard_demo_task(void *pvParameter) {

  // demo counter (written to file)
  int counter = 0;

  while (1) {
    xSemaphoreTakeT(spiBus.mutex);
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