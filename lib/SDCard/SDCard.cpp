//
// SD Card
//

#include <definitions.h>

// standard libraries
#include <fmt/core.h>
#include <iostream>
#include <stdio.h>
#include <string>

#include <FS.h>
#include <SD.h> // sd card
#include <SD_MMC.h>
#include <SPI.h>

#include <DriverDisplay.h>
#include <Helper.h>
#include <SDCard.h>
#include <SPIBus.h>

#define FILENAME "/ser4data.log"

extern SPIBus spiBus;
extern SDCard sdCard;
extern DriverDisplay driverDisplay;

void SDCard::re_init() { init(); }

void printDirectory(File dir, int numTabs) {
  while (true) {

    File entry = dir.openNextFile();
    if (!entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      cout << '\t';
    }
    cout << entry.name();
    if (entry.isDirectory()) {
      cout << "/" << endl;
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      cout << "\t\t" << entry.size() << endl;
    }
    entry.close();
  }
}

void SDCard::init() {
  inited = false;

  string s = "[?] Init 'SDCard'...\n";
  cout << s;
  driverDisplay.print(s.c_str());

  if (!SD.begin(SPI_CS_SDCARD, spiBus.spi)) {
    cout << "initialization failed!";
    return;
  }

  s = "[v] SDCard initialized.\n";
  cout << s;
  driverDisplay.print(s.c_str());
  cout << "    SD-Card content:" << endl;
  File root = SD.open("/");
  printDirectory(root, 1);
  s = fmt::format("   Open file '{}' for append...", FILENAME);
  cout << s;
  driverDisplay.print(s.c_str());

  xSemaphoreTakeT(spiBus.mutex);
  // open file
  dataFile = SD.open(FILENAME, FILE_APPEND); // mode: APPEND: FILE_APPEND, OVERWRITE: FILE_WRITE

  if (dataFile == 0) {
    cout << "failed." << endl;
  } else {
    cout << "ok." << endl;
  }
  time_t theTime = time(NULL);
  struct tm t = *localtime(&theTime);
  string msg = fmt::format("Start log SD Card at {}\n", asctime(&t));
  dataFile.printf(msg.c_str());
  dataFile.flush();
  xSemaphoreGive(spiBus.mutex);

  inited = true;
  s = fmt::format("   ok.");
  cout << s;
  driverDisplay.print(s.c_str());
}

void SDCard::write(string msg) {
  if (sdCard.isInited() && sdCard.dataFile) {
    xSemaphoreTakeT(spiBus.mutex);
    sdCard.dataFile.print(msg.c_str());
    sdCard.dataFile.flush();
    xSemaphoreGive(spiBus.mutex);
  }
}
