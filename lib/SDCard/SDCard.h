//
// SD Card
//

#ifndef SOLAR_CAR_CONTROL_SYSTEM_SDCARD_H
#define SOLAR_CAR_CONTROL_SYSTEM_SDCARD_H

#include <SD.h>

#define FILENAME "/ser4data.csv"

class SDCard {
private:
  bool mounted;
  void printDirectory(File dir, int numTabs);
  File dataFile;

public:
  bool logEnabled;
  void init();
  void re_init();
  bool isMounted() { return mounted; }
  bool isReadyForLog() { return mounted && dataFile != 0; }
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
