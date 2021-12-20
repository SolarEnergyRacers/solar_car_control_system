//
// SD Card
//

#ifndef SOLAR_CAR_CONTROL_SYSTEM_SDCARD_H
#define SOLAR_CAR_CONTROL_SYSTEM_SDCARD_H

#include <SD.h>

void write_sdcard_demo_task(void *pvParameter); // TODO: not sure yet if we want a reader/writer task that feeds/consumes from/to queues, or
                                                // just offer functionality to other tasks

class SDCard {
private:
  bool inited;

public:
  File dataFile;
  void init();
  void re_init();
  bool isInited() { return inited; }
  //void write(char *msg);
  void write(string msg);
};
#endif // SOLAR_CAR_CONTROL_SYSTEM_SDCARD_H
