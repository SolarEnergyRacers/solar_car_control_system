//
// Display
//

#ifndef SOLAR_CAR_CONTROL_SYSTEM_DISPLAY_H
#define SOLAR_CAR_CONTROL_SYSTEM_DISPLAY_H

#include <Wire.h>
#include <abstract_task.h>
#include <definitions.h>

class Display : public abstract_task {
private:
  //    Adafruit_SSD1305 display = Adafruit_SSD1305(OLED_WIDTH, OLED_HEIGHT, &Wire, OLED_RESET); // TODO: throws error: redefinition of
  //    'class Adafruit_SSD1305'
public:
  string getName(void) { return "Display"; };
  void init(void);
  void re_init(void);
  void exit(void);
  void task(void);
};

#endif // SOLAR_CAR_CONTROL_SYSTEM_ DISPLAY_H
