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
#include <SD.h>
#include <SD_MMC.h>
#include <SPI.h>

#include <ADS1X15.h>
#include <Adafruit_ILI9341.h>
#include <CarState.h>
#include <Display.h>
#include <DriverDisplay.h>
#include <Helper.h>
#include <SDCard.h>
#include <SPIBus.h>

extern SPIBus spiBus;
extern SDCard sdCard;
extern DriverDisplay driverDisplay;

void SDCard::re_init() { init(); }

string SDCard::directory() {
  if (!sdCard.isInited()) {
    re_init();
  }
  stringstream ss("SD-Card content:\n");
  File root = SD.open("/");
  printDirectory(root, 1);
  root.close();
  ss << "~~~~~~~~~~~~~~~~" << endl;
  return ss.str();
}

void SDCard::printDirectory(File dir, int numTabs) {
  while (true) {
    File entry = dir.openNextFile();
    if (!entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      cout << '  '; // TAB size: 2
    }
    cout << entry.name();
    if (entry.isDirectory()) {
      cout << "/" << endl;
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      cout << " [" << entry.size() << "]" << endl;
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
    cout << "initialization failed!" << endl;
    return;
  }

  s = "[v] SDCard initialized.\n";
  cout << s;
  driverDisplay.print(s.c_str());
  s = fmt::format("   Open file '{}' for append...", FILENAME);
  cout << s;
  driverDisplay.print(s.c_str());

  xSemaphoreTakeT(spiBus.mutex);
  // open log file
  dataFile = SD.open(FILENAME, FILE_APPEND); // mode: APPEND: FILE_APPEND, OVERWRITE: FILE_WRITE
  xSemaphoreGive(spiBus.mutex);
  if (dataFile == 0) {
    cout << "failed." << endl;
  } else {
    cout << "ok." << endl;
    inited = true;
    write(carState.csv("Initial State", true));
  }
  s = fmt::format("   ok.\n");
  cout << s;
  driverDisplay.print(s.c_str());
}

void SDCard::write(string msg) {
  if (sdCard.isInited() && sdCard.dataFile) {
    xSemaphoreTakeT(spiBus.mutex);
    try {
      dataFile.print(msg.c_str());
      dataFile.flush();
    } catch (exception &ex) {
      cout << "ERROR writing SD card: " << ex.what() << endl;
      unmount();
    }
    xSemaphoreGive(spiBus.mutex);
  } else {
    re_init();
  }
}

void SDCard::unmount() {
  if (sdCard.isInited() && sdCard.dataFile) {
    xSemaphoreTakeT(spiBus.mutex);
    try {
      dataFile.flush();
      dataFile.close();
      SD.end();
      xSemaphoreGive(spiBus.mutex);
      cout << "SD card unmounted." << endl;
    } catch (exception &ex) {
      cout << "ERROR unmouning SD card: " << ex.what() << endl;
    }
    inited = false;
  }
}
