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

void SDCard::init() {
  logEnabled = false;
  string s = "[?] Init 'SDCard'...\n";
  cout << s;
  driverDisplay.print(s.c_str());

  if (!mount()) {
    cout << "initialization failed!" << endl;
    return;
  }
  s = "[v] SDCard initialized.\n";
  cout << s;
  driverDisplay.print(s.c_str());

  s = fmt::format("   Open '{}' (append)...", FILENAME);
  cout << s;
  driverDisplay.print(s.c_str());

  if (open_log_file()) {
    logEnabled = true;
    write(carState.csv("Initial State", true));
    cout << "ok." << endl;
    driverDisplay.print("SD Card mounted.\n");
  } else {
    cout << "failed." << endl;
  }
}

bool SDCard::mount() {
  try {
    xSemaphoreTakeT(spiBus.mutex);
    mounted = SD.begin(SPI_CS_SDCARD, spiBus.spi);
    xSemaphoreGive(spiBus.mutex);
    if (mounted) {
      return true;
    }
    cout << "ERROR mouning SD card." << endl;
  } catch (exception &ex) {
    xSemaphoreGive(spiBus.mutex);
    cout << "ERROR mouning SD card: " << ex.what() << endl;
  }
  return false;
}

bool SDCard::open_log_file() {
  if (!mounted) {
    // give it a shot
    mount();
  }
  if (mounted) {
    try {
      xSemaphoreTakeT(spiBus.mutex);
      dataFile = SD.open(FILENAME, FILE_APPEND); // mode: APPEND: FILE_APPEND, OVERWRITE: FILE_WRITE
      xSemaphoreGive(spiBus.mutex);
      if (dataFile != 0) {
        return true;
      }
      cout << "ERROR opening '" << FILENAME << "'" << endl;
    } catch (exception &ex) {
      xSemaphoreGive(spiBus.mutex);
      cout << "ERROR opening '" << FILENAME << "': " << ex.what() << endl;
    }
  }
  return false;
}

void SDCard::unmount() {
  if (isReadyForLog()) {
    try {
      xSemaphoreTakeT(spiBus.mutex);
      dataFile.flush();
      dataFile.close();
      SD.end();
      xSemaphoreGive(spiBus.mutex);
      cout << "Log file closed." << endl;
    } catch (exception &ex) {
      xSemaphoreGive(spiBus.mutex);
      cout << "ERROR closing log file: " << ex.what() << endl;
    }
  }
  if (isMounted()) {
    try {
      xSemaphoreTakeT(spiBus.mutex);
      SD.end();
      xSemaphoreGive(spiBus.mutex);
      cout << "SD card unmounted." << endl;
    } catch (exception &ex) {
      xSemaphoreGive(spiBus.mutex);
      cout << "ERROR unmouning SD card: " << ex.what() << endl;
    }
    mounted = false;
  }
  logEnabled = false;
}

string SDCard::directory() {
  if (!isMounted()) {
    mount();
  }

  if (isMounted()) {
    stringstream ss("SD-Card content:\n");
    File root = SD.open("/");
    printDirectory(root, 1);
    root.close();
    ss << "~~~~~~~~~~~~~~~~" << endl;
    return ss.str();
  }
  return "ERROR reading SD card.";
}

void SDCard::printDirectory(File dir, int numTabs) {
  while (true) {
    File entry = dir.openNextFile();
    if (!entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      cout << '\t'; // TAB size: 2
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

void SDCard::write(string msg) {
  if (logEnabled) {
    if (!isReadyForLog()) {
      // give it a shot
      open_log_file();
    }
  }
  if (isReadyForLog()) {
    try {
      xSemaphoreTakeT(spiBus.mutex);
      dataFile.print(msg.c_str());
      dataFile.flush();
      xSemaphoreGive(spiBus.mutex);
    } catch (exception &ex) {
      xSemaphoreGive(spiBus.mutex);
      mounted = false; // prepare for complete re_init
      cout << "ERROR writing SD card: " << ex.what() << endl;
    }
  }
}
