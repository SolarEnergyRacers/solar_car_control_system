//
// Display
//

#ifndef DISPLAY_LARGE_H
#define DISPLAY_LARGE_H

void init_display_large(void);
void draw_display_large_demo_task(void *pvParameter);
void draw_display_large_background_task(void *pvParameter);
void calc_and_draw_display_background();
void write_speed(int speed, int color);
void turn_indicator(char kind);
void draw_info_border(int color);

char getIndicatorDirection();
void setIndicatorDirection(char direction);
bool getIndicatorState();
void setIndicatorState(bool state);

#define TFT_DC 12
#define TFT_CS 5
#define TFT_MOSI 23
#define TFT_CLK 18
#define TFT_RST 17
#define TFT_MISO 21


#endif // DISPLAY_LARGE_H
