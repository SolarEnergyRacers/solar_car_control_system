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
#include <Console.h>
#include <Display.h>
#include <DriverDisplay.h>
#include <Helper.h>
#include <SDCard.h>
#include <SPIBus.h>

extern Console console;
extern SPIBus spiBus;
extern SDCard sdCard;
extern DriverDisplay driverDisplay;

void SDCard::re_init() { init(); }

void SDCard::init() {
  logEnabled = false;
  string s = "[?] Init 'SDCard'...\n";
  console << s;
  driverDisplay.print(s.c_str());

  if (!mount()) {
    console << "initialization failed!\n";
    return;
  }
  s = "[v] SDCard initialized.\n";
  console << s;
  driverDisplay.print(s.c_str());

  s = fmt::format("    Open '{}' (append)...", FILENAME);
  console << s;
  driverDisplay.print(s.c_str());

  if (open_log_file()) {
    logEnabled = true;
    write(carState.csv("Initial State", true));
    console << "ok.\n";
    driverDisplay.print("SD Card mounted.\n");
  } else {
    console << "failed.\n";
  }
}

bool SDCard::mount() {
  try {
    xSemaphoreTakeT(spiBus.mutex);
    mounted = SD.begin(SPI_CS_SDCARD, spiBus.spi);
    xSemaphoreGive(spiBus.mutex);
    if (mounted) {
      console << "SD card mounted.\n";
      return true;
    }
    console << "ERROR mounting SD card.\n";
  } catch (exception &ex) {
    xSemaphoreGive(spiBus.mutex);
    console << "ERROR mounting SD card: " << ex.what() << "\n";
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
        console << "Log file opend for append.\n";
        return true;
      }
      console << "ERROR opening '" << FILENAME << "'\n";
    } catch (exception &ex) {
      xSemaphoreGive(spiBus.mutex);
      console << "ERROR opening '" << FILENAME << "': " << ex.what() << "\n";
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
      console << "Log file closed.\n";
    } catch (exception &ex) {
      xSemaphoreGive(spiBus.mutex);
      console << "ERROR closing log file: " << ex.what() << "\n";
    }
  }
  if (isMounted()) {
    try {
      xSemaphoreTakeT(spiBus.mutex);
      SD.end();
      xSemaphoreGive(spiBus.mutex);
      console << "SD card unmounted.\n";
    } catch (exception &ex) {
      xSemaphoreGive(spiBus.mutex);
      console << "ERROR unmounting SD card: " << ex.what() << "\n";
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
    ss << "~~~~~~~~~~~~~~~~\n";
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
      console << '\t'; // TAB size: 2
    }
    console << entry.name();
    if (entry.isDirectory()) {
      console << "/\n";
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      console << " [" << entry.size() << "]\n";
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
      console << "ERROR writing SD card: " << ex.what() << "\n";
    }
  }
}
