//
// SD Card
//

#ifndef SOLAR_CAR_CONTROL_SYSTEM_SDCARD_H
#define SOLAR_CAR_CONTROL_SYSTEM_SDCARD_H

#include <SD.h>

#define FILENAME "/ser4data.log"

class SDCard {
private:
  bool inited;
  void printDirectory(File dir, int numTabs);
  File dataFile;

public:
  void init();
  void re_init();
  bool isInited() { return inited; }
  // write a string into the dataFile
  void write(string msg);
  // prints the directory tree of the card
  string directory();
  // unmounts the card and ends the SD
  void unmount();
};
#endif // SOLAR_CAR_CONTROL_SYSTEM_SDCARD_H
