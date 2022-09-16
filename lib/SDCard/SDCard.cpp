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
#include <IOExt.h>
#include <SDCard.h>
#include <SPIBus.h>

extern Console console;
extern SPIBus spiBus;
extern SDCard sdCard;
extern DriverDisplay driverDisplay;
extern IOExt ioExt;

string SDCard::re_init() { return init(); }

string SDCard::init() {
  bool hasError = false;
  console << "[  ] Init 'SDCard'...\n";

  if (!mount())
    hasError = true;

  return fmt::format("[{}] SDCard initialized.", hasError ? "--" : "ok");
}

// https://github.com/espressif/arduino-esp32/issues/5676
// https://github.com/espressif/arduino-esp32/blob/master/libraries/SPI/examples/SPI_Multiple_Buses/SPI_Multiple_Buses.ino

bool SDCard::mount() {
  if (isMounted()) {
    return true;
  }
  if (!carState.SdCardDetect) {
    ioExt.readAllPins();
    carState.SdCardDetect = carState.getPin(PinSdCardDetect)->value == 1;
  }
  if (!carState.SdCardDetect) {
    console << "     No SD card detected!\n";
    mounted = false;
    xSemaphoreTakeT(spiBus.mutex);
    SD.end();
    xSemaphoreGive(spiBus.mutex);
    return false;
  }
  try {
    console << "     Mounting SD card ...\n";
    xSemaphoreTakeT(spiBus.mutex);
    // mounted = SD.begin(SPI_CS_SDCARD, spiBus.spi, 400000U, "/", 10); fails!
    mounted = SD.begin(SPI_CS_SDCARD, spiBus.spi);
    xSemaphoreGive(spiBus.mutex);
    if (mounted) {
      console << "     SD card mounted.\n";
      uint8_t cardType = SD.cardType();
      if (cardType == CARD_NONE) {
        console << "No SD card attached.\n";
        return false;
      }

      console << "SD Card Type: ";
      if (cardType == CARD_MMC) {
        console << "MMC";
      } else if (cardType == CARD_SD) {
        console << "SDSC";
      } else if (cardType == CARD_SDHC) {
        console << "SDHC";
      } else {
        console << "UNKNOWN";
      }

      uint64_t cardSize = SD.cardSize() / (1024 * 1024);
      console << "SD Card Size: " << cardSize << "MB\n";

      return true;
    }
    console << "     ERROR: Unable to mount SD card.\n";
  } catch (exception &ex) {
    xSemaphoreGive(spiBus.mutex);
    console << "     ERROR: Unable to mount SD card: " << ex.what() << "\n";
  }
  return false;
}

bool SDCard::open_log_file() {
  if (carState.LogFilename.length() > 0 && mount()) {
    try {
      xSemaphoreTakeT(spiBus.mutex);
      dataFile = SD.open(carState.LogFilename.c_str(), FILE_APPEND); // mode: APPEND: FILE_APPEND, OVERWRITE: FILE_WRITE
      xSemaphoreGive(spiBus.mutex);
      if (dataFile != 0) {
        console << "     Log file opend for append.\n";
        return true;
      }
      console << "     ERROR opening '" << carState.LogFilename << "'\n";
    } catch (exception &ex) {
      xSemaphoreGive(spiBus.mutex);
      console << "     ERROR opening '" << carState.LogFilename << "': " << ex.what() << "\n";
    }
  } else {
    console << "ERROR at open_log_file: SD card not mounted.\n";
  }
  return false;
}

void SDCard::unmount() {
  if (!carState.SdCardDetect) {
    mounted = false;
    return;
  }
  if (isReadyForLog()) {
    try {
      xSemaphoreTakeT(spiBus.mutex);
      dataFile.flush();
      dataFile.close();
      SD.end();
      xSemaphoreGive(spiBus.mutex);
      console << "     Log file closed.\n";
    } catch (exception &ex) {
      xSemaphoreGive(spiBus.mutex);
      console << "     ERROR closing log file: " << ex.what() << "\n";
    }
  }
  if (isMounted()) {
    try {
      xSemaphoreTakeT(spiBus.mutex);
      SD.end();
      xSemaphoreGive(spiBus.mutex);
      console << "     SD card unmounted.\n";
    } catch (exception &ex) {
      xSemaphoreGive(spiBus.mutex);
      console << "     ERROR unmounting SD card: " << ex.what() << "\n";
    }
    mounted = false;
  }
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
  return "ERROR at directory: SD card not mounted.";
}

void SDCard::printDirectory(File dir, int numTabs) {
  while (true) {
    File entry = dir.openNextFile();
    if (!entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      console << "  "; // TAB size: 2
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
  if (!isReadyForLog()) {
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
      console << "     ERROR writing SD card: " << ex.what() << "\n";
    }
  }
}
