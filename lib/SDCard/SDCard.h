//
// SD Card
//

#ifndef SOLAR_CAR_CONTROL_SYSTEM_SDCARD_H
#define SOLAR_CAR_CONTROL_SYSTEM_SDCARD_H

#include <CarState.h>
#include <SD.h>

extern CarState carState;

class SDCard {
private:
  bool mounted = false;
  File dataFile;

  void printDirectory(File dir, int numTabs);

public:
  string init();
  string re_init();
  bool isMounted() { return carState.SdCardDetect && mounted; }
  bool isReadyForLog() { return isMounted() && dataFile != 0; }
  // write a string into the dataFile
  void write(string msg);
  // prints the directory tree of the card
  string directory();
  // mount the card
  bool mount();
  // prepare log
  bool open_log_file();
  // close log file, unmount the card, end the SD and disable logging
  void unmount();
};
#endif // SOLAR_CAR_CONTROL_SYSTEM_SDCARD_H
