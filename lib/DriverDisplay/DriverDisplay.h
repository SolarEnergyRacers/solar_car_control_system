//
// Display
//

#ifndef DRIVER_DISPLAY_H
#define DRIVER_DISPLAY_H

#define ILI9341 // (320x240)

#include <Adafruit_ILI9341.h> // placed here for display colors in other moduls
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

extern SemaphoreHandle_t spi_mutex;

// public structures
enum INDICATOR {
  INDICATOR_OFF,
  INDICATOR_LEFT,
  INDICATOR_RIGHT,
  INDICATOR_WARN
};
enum INFO_TYPE {
  INFO_TYPE_INFO,   // INFO = ILI9341_WHITE
  INFO_TYPE_STATUS, // STATUS = ILI9341_GREEN
  INFO_TYPE_WARN,   // WARN = ILI9341_PURPLE
  INFO_TYPE_ERROR   // ILI9341_RED
};
// =
// public functions
void draw_display_border(int color);

void write_driver_info(String msg, INFO_TYPE type);
void write_speed(int speed);
void write_bat(float voltage);
void write_motor(float ampers);
void write_pv(float voltage);
void write_acceleration(int value);

void indicator(INDICATOR direction);
void light1OnOff();
void light2OnOff();

void arrow_increase(bool on);
void arrow_decrease(bool on);

void driver_display_demo_screen();

// internal functions for inner task communication
INDICATOR getIndicatorDirection();
void indicator_set_and_blink(INDICATOR direction, bool blinkOn);

// FreeRTOS - start
bool init_driver_display(void);
void driver_display_task(void *pvParameter);
// FreeRTOS - end

#endif // DRIVER_DISPLAY_H
