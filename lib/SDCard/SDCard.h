//
// SD Card
//

#ifndef SOLAR_CAR_CONTROL_SYSTEM_SDCARD_H
#define SOLAR_CAR_CONTROL_SYSTEM_SDCARD_H

#include <SD.h>

class SDCard {
private:
  bool inited;

public:
  File dataFile;
  void init();
  void re_init();
  bool isInited() { return inited; }
  void write(string msg);
};
#endif // SOLAR_CAR_CONTROL_SYSTEM_SDCARD_H
