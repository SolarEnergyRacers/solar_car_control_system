//
// Display
//

#ifndef DISPLAY_H
#define DISPLAY_H

#include <definitions.h>
#include <Wire.h>
#include <abstract_task.h>

class Display : public abstract_task {
private:
//    Adafruit_SSD1305 display = Adafruit_SSD1305(OLED_WIDTH, OLED_HEIGHT, &Wire, OLED_RESET); // TODO: throws error: redefinition of 'class Adafruit_SSD1305'
public:
    string getName(void) { return "Display"; };
    void init(void);
    void re_init(void);
    void exit(void);
    void task(void);
};

#endif // DISPLAY_H
