//
// Display
//

#ifndef DRIVER_DISPLAY_H
#define DRIVER_DISPLAY_H

#define ILI9341 // (320x240)

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

extern SemaphoreHandle_t spi_mutex;

// RTOS task
void init_driver_display(void);
void driver_display_task(void *pvParameter);
void draw_display_large_demo_task(void *pvParameter);

// public functions
void draw_display_background();
void write_speed(int speed, int color);
void turn_indicator(char kind);
void show_Faster(bool on);
void show_Slower(bool on);


// internal functions for inner task communication
char getIndicatorDirection();
void setIndicatorDirection(char direction);
bool getIndicatorState();
void setIndicatorState(bool state);

#endif // DRIVER_DISPLAY_H
