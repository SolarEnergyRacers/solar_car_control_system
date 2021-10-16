//
// Display
//

#ifndef SER_ENGINEER_DISPLAY_H
#define SER_ENGINEER_DISPLAY_H

#include <Wire.h>
#include <abstract_task.h>
#include <definitions.h>

class EngineerDisplay : public abstract_task {
private:
  //    Adafruit_SSD1305 display = Adafruit_SSD1305(OLED_WIDTH, OLED_HEIGHT, &Wire, OLED_RESET); // TODO: throws error: redefinition of
  //    'class Adafruit_SSD1305'
public:
  string getName(void) { return "EngineerDisplay"; };
  void init(void);
  void re_init(void);
  void exit(void);
  void task(void);
};

#endif //SER_ENGINEER_DISPLAY_H DISPLAY_H
