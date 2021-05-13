/***
 *
 * Driver's Display
 *
 * The driver's display is passive with the exception of the indicator function.
 * This means that all sensors send their values to the display.
 *
 ***/

#include "../../include/definitions.h"

#include "DriverDisplay.h"
#include <SPIBus.h>

#include <Adafruit_GFX.h>     // graphics library
#include <Adafruit_ILI9341.h> // display
#include <Fonts/FreeMono12pt7b.h>
#include <Fonts/FreeMono18pt7b.h>
#include <Fonts/FreeMono24pt7b.h>
#include <Fonts/FreeMono9pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSans18pt7b.h>
#include <Fonts/FreeSans24pt7b.h>
#include <Fonts/FreeSans9pt7b.h>

Adafruit_ILI9341 tft =
    Adafruit_ILI9341(SPI_CS_TFT, SPI_DC, SPI_MOSI, SPI_CLK, SPI_RST, SPI_MISO);

//==== Driver Display definition ==== START
// display formats and sizes
int bgColor = 0x000000;
int infoFrameX = 0;
int infoFrameY = 0;
int infoFrameSizeX = -1; // full tft width, calculated beow
int infoFrameSizeY = 64;
int infoTextSize = 3;

int mainFrameX =
    infoFrameSizeY; // frame around value display (exclude text message lines)

// speed display
int speedFrameX = -1; // get calculated later: (sizeX - speedFrameCx) / 2;
int speedFrameY = 80;
int speedFrameSizeX = 156;
int speedFrameSizeY = 76;
int speedTextSize = 8;

// acceleration display
int accFrameX = 2;
int accFrameY = 118;    // speedFrameY + speedFrameSizeY / 2
int accFrameSizeX = -1; // get calculated later: speedFrameX - 4
int accFrameSizeY = 38; // speedFrameSizeY / 2
int accTextSize = 4;    // speedTextSize / 2

// ---- voltage and current displays ---- START
int labelLen = 9; // label length for all 3 voltage/current displays

// battery voltage display
int batFrameX = 10;
int batFrameY = 180;
int batTextSize = 2;
int lightTextSize = 2;

// photovoltaics voltage display
int pvFrameX = 10;
int pvFrameY = 200;
int pvTextSize = 2;

// motor current display
int motorFrameX = 10;
int motorFrameY = 220;
int motorTextSize = 2;
// ---- voltage and current displays ---- END

// turn indicator arrows
int indicatorLeftX = 10;
int indicatorY = 92;
int indicatorRightX = 310;
int indicatorWidth = 30;
int indicatorHeight = 20;

// light on indicator
int light1OnX = 250;
int light1OnY = 118;
int light2OnX = 250;
int light2OnY = 138;

// life sign for connection to microprocessor via rtx
int lifeSignX = -1;
int lifeSignY = -1;
int lifeSignRadius = 4;
//==== Driver Display definition ==== END

//==== display cache =====================
// ... to avoid flickering
int lifeSignCounter = 0;
bool lifeSignState = false;
String infoLast = "";
int speedLast = -1;
int accelerationLast = -1;
float batLast = -1;
float pvLast = -1;
float motorLast = -1;
bool light1On = false;
bool light2On = false;
//=======================================

// writes float value  in the range from -9999.9 to 9999.9
float _write_float(int x, int y, float valueLast, float value, int textSize,
                   int color) {

  if (value < -9999.9 || value > 9999.9) {
    printf("ERROR: call _write_float with a value outside the range: '%f'\n",
           value);
    return value;
  }
  int digitWidth = textSize * 6;
  int digitHeight = textSize * 8;
  // determine the sign of new and old value
  char sign = value < 0 ? '-' : '+';
  char signOld = valueLast < 0 ? '-' : '+';
  // determine the four digits and one decimal of the new value
  float val = abs(value);
  int d1 = (int)val % 10;
  int d2 = ((int)val / 10) % 10;
  int d3 = ((int)val / 100) % 10;
  int d4 = ((int)val / 1000) % 10;
  int d0 = (val - (int)val) * 10;
  // determine the four digits and one decimal of the stored, old value
  float valOld = abs(valueLast);
  int d1o = (int)valOld % 10;
  int d2o = ((int)valOld / 10) % 10;
  int d3o = ((int)valOld / 100) % 10;
  int d4o = ((int)valOld / 1000) % 10;
  int d0o = (valOld - (int)valOld) * 10;

  // CRITICAL SECTION SPI: start
  xSemaphoreTake(spi_mutex, portMAX_DELAY);

  tft.setTextSize(textSize);
  tft.setTextColor(color);
  tft.setCursor(x, y);
  // if a change in the digit then replace the old with new value by
  // first deleting the digit area and second write the new value
  if (d0 != d0o) {
    tft.fillRect(x + (digitWidth + 1) * 5, y, digitWidth * 2, digitHeight,
                 bgColor);
    tft.setCursor(x + (digitWidth + 1) * 5, y);
    tft.printf(".%d", d0);
  }
  if (d1 != d1o) {
    tft.fillRect(x + (digitWidth + 1) * 4, y, digitWidth, digitHeight, bgColor);
    tft.setCursor(x + (digitWidth + 1) * 4, y);
    tft.print(d1);
  }
  if (d2 != d2o) {
    tft.fillRect(x + (digitWidth + 1) * 3, y, digitWidth, digitHeight, bgColor);
    tft.setCursor(x + (digitWidth + 1) * 3, y);
    if (abs(value) > 9) {
      tft.print(d2);
    }
  }
  if (d3 != d3o) {
    tft.fillRect(x + (digitWidth + 1) * 2, y, digitWidth, digitHeight, bgColor);
    tft.setCursor(x + (digitWidth + 1) * 2, y);
    if (abs(value) > 99) {
      tft.print(d3);
    }
  }
  if (d4 != d4o) {
    tft.fillRect(x + (digitWidth + 1) * 1, y, digitWidth, digitHeight, bgColor);
    tft.setCursor(x + (digitWidth + 1) * 1, y);
    if (abs(value) > 999) {
      tft.print(d4);
    }
  }
  if (sign != signOld) {
    tft.fillRect(x, y, (digitWidth + 1), digitHeight, bgColor);
    tft.setCursor(x, y);
    tft.print(sign);
  }

  xSemaphoreGive(spi_mutex);
  // CRITICAL SECTION SPI: end

  return value;
}

// writes integer value in the range from 0 to 999
int _write_int(int x, int y, int valueLast, int value, int textSize,
               int color) {
  if (value < 0 || value > 999) {
    printf("ERROR: call _write_int with a value outside the range: '%d'",
           value);
    return value;
  }
  int digitWidth = textSize * 6;
  int digitHeight = textSize * 8;

  // determine the three digits the stored, new value
  int d1 = (int)value % 10;
  int d2 = ((int)value / 10) % 10;
  int d3 = ((int)value / 100) % 10;
  // determine the three digits the stored, old value
  int d1o = (int)valueLast % 10;
  int d2o = ((int)valueLast / 10) % 10;
  int d3o = ((int)valueLast / 100) % 10;

  // CRITICAL SECTION SPI: start
  xSemaphoreTake(spi_mutex, portMAX_DELAY);

  tft.setTextSize(textSize);
  tft.setTextColor(color);
  tft.setCursor(x, y);
  // if a change in the digit then replace the old with new value by
  // first deleting the digit area and second write the new value
  if (d1 != d1o) {
    tft.fillRect(x + (digitWidth + 1) * 2, y, digitWidth, digitHeight, bgColor);
    tft.setCursor(x + (digitWidth + 1) * 2, y);
    tft.print(d1);
  }
  if (d2 != d2o) {
    tft.fillRect(x + (digitWidth + 1) * 1, y, digitWidth, digitHeight, bgColor);
    tft.setCursor(x + (digitWidth + 1) * 1, y);
    if (abs(value) > 9) {
      tft.print(d2);
    }
  }
  if (d3 != d3o) {
    tft.fillRect(x + (digitWidth + 1) * 0, y, digitWidth, digitHeight, bgColor);
    tft.setCursor(x + (digitWidth + 1) * 0, y);
    if (abs(value) > 99) {
      tft.print(d3);
    }
  }

  xSemaphoreGive(spi_mutex);
  // CRITICAL SECTION SPI: end

  return value;
}

// write color of the border of the main display
void draw_display_border(int color) {
  // CRITICAL SECTION SPI: start
  xSemaphoreTake(spi_mutex, portMAX_DELAY);

  tft.drawRoundRect(0, mainFrameX, tft.width(), tft.height() - mainFrameX, 8,
                    color);

  xSemaphoreGive(spi_mutex);
  // CRITICAL SECTION SPI: end
}

// write color of the border of the speed display
void draw_speed_border(int color) {
  // CRITICAL SECTION SPI: start
  xSemaphoreTake(spi_mutex, portMAX_DELAY);

  tft.drawRoundRect(speedFrameX, speedFrameY, speedFrameSizeX, speedFrameSizeY,
                    4, color);

  xSemaphoreGive(spi_mutex);
  // CRITICAL SECTION SPI: end
}

// write color of the border of the speed display
void draw_acceleration_border(int color) {
  // CRITICAL SECTION SPI: start
  xSemaphoreTake(spi_mutex, portMAX_DELAY);

  accFrameSizeX = speedFrameX - 3;
  tft.drawRoundRect(accFrameX, accFrameY, accFrameSizeX, accFrameSizeY, 4,
                    color);

  xSemaphoreGive(spi_mutex);
  // CRITICAL SECTION SPI: end
}

void lifeSign() {
  int color;
  if (lifeSignState) {
    color = bgColor;
  } else {
    color = ILI9341_GREEN;
  }

  // CRITICAL SECTION SPI: start
  xSemaphoreTake(spi_mutex, portMAX_DELAY);

  lifeSignX = tft.width() - lifeSignRadius - 6;
  lifeSignY = tft.height() - lifeSignRadius - 6;
  tft.fillCircle(lifeSignX, lifeSignY, lifeSignRadius, color);

  xSemaphoreGive(spi_mutex);
  // CRITICAL SECTION SPI: end

  lifeSignState = !lifeSignState;
}

void draw_display_background() {
  // CRITICAL SECTION SPI: start
  xSemaphoreTake(spi_mutex, portMAX_DELAY);

  tft.setRotation(0);
  tft.fillScreen(bgColor);
  tft.setRotation(1);
  tft.setTextSize(2);
  tft.setTextColor(ILI9341_DARKGREEN);
  tft.setCursor(batFrameX, batFrameY);
  tft.print("  Bat(V):");

  tft.setCursor(pvFrameX, pvFrameY);
  tft.print("   PV(A):");

  tft.setCursor(motorFrameX, motorFrameY);
  tft.print("Motor(A):");

  xSemaphoreGive(spi_mutex);
  // CRITICAL SECTION SPI: end

  infoFrameSizeX = tft.width();
  speedFrameX = (tft.width() - speedFrameSizeX) / 2;

  draw_display_border(ILI9341_GREEN);
  draw_speed_border(ILI9341_YELLOW);
  draw_acceleration_border(ILI9341_YELLOW);
}

void _arrow_increase(int color) {
  int x = speedFrameX;
  int y = speedFrameY - 3;

  // CRITICAL SECTION SPI: start
  xSemaphoreTake(spi_mutex, portMAX_DELAY);

  tft.fillTriangle(x, y, x + speedFrameSizeX, y, x + speedFrameSizeX / 2,
                   y - 10, color);

  xSemaphoreGive(spi_mutex);
  // CRITICAL SECTION SPI: end
}

void _arrow_decrease(int color) {
  int x = speedFrameX;
  int y = speedFrameY + speedFrameSizeY + 3;

  // CRITICAL SECTION SPI: start
  xSemaphoreTake(spi_mutex, portMAX_DELAY);

  tft.fillTriangle(x, y, x + speedFrameSizeX, y, x + speedFrameSizeX / 2,
                   y + 10, color);

  xSemaphoreGive(spi_mutex);
  // CRITICAL SECTION SPI: end
}

// show the slower arrow (red under the speed display)
void arrow_decrease(bool on) {
  int color = bgColor;
  if (on) {
    arrow_increase(false);
    color = ILI9341_RED;
  }
  _arrow_decrease(color);
}

// show the faster arrow (green above the speed display)
void arrow_increase(bool on) {
  int color = bgColor;
  if (on) {
    arrow_decrease(false);
    color = ILI9341_YELLOW;
  }
  _arrow_increase(color);
}

void _turn_Left(int color) {
  int x = indicatorLeftX;
  int y = indicatorY;
  tft.fillTriangle(x, y, x + indicatorWidth, y - indicatorHeight,
                   x + indicatorWidth, y + indicatorHeight, color);
}

void _turn_Right(int color) {
  int x = indicatorRightX;
  int y = indicatorY;
  tft.fillTriangle(x, y, x - indicatorWidth, y - indicatorHeight,
                   x - indicatorWidth, y + indicatorHeight, color);
}

void indicator_set_and_blink(INDICATOR direction) {
  indicator_set_and_blink(direction, true);
}

void indicator_set_and_blink(INDICATOR direction, bool blinkOn) {
  // CRITICAL SECTION SPI: start
  xSemaphoreTake(spi_mutex, portMAX_DELAY);

  _turn_Left(bgColor);
  _turn_Right(bgColor);
  if (blinkOn) {
    switch (direction) {
    case INDICATOR::LEFT:
      _turn_Left(ILI9341_YELLOW);
      break;

    case INDICATOR::RIGHT:
      _turn_Right(ILI9341_YELLOW);
      break;

    case INDICATOR::WARN:
      _turn_Left(ILI9341_RED);
      _turn_Right(ILI9341_RED);
      break;

    case INDICATOR::OFF:
    default:
      break;
    }
  }

  xSemaphoreGive(spi_mutex);
  // CRITICAL SECTION SPI: end
}

void _light1(bool lightOn) {
  light1On = !lightOn;
  light1OnOff();
}

void _light2(bool lightOn) {
  light2On = !lightOn;
  light2OnOff();
}

void light1OnOff() {
  int color = ILI9341_YELLOW;
  if (light1On) {
    color = bgColor;
    _light2(false);
  }

  light1On = !light1On;

  // CRITICAL SECTION SPI: start
  xSemaphoreTake(spi_mutex, portMAX_DELAY);

  tft.setTextColor(color);
  tft.setTextSize(lightTextSize);
  tft.setCursor(light1OnX, light1OnY);
  tft.print("Light");

  xSemaphoreGive(spi_mutex);
  // CRITICAL SECTION SPI: end
}

void light2OnOff() {
  int color = ILI9341_BLUE;
  if (light2On) {
    color = bgColor;
  } else {
    _light1(true);
  }
  light2On = !light2On;

  // CRITICAL SECTION SPI: start
  xSemaphoreTake(spi_mutex, portMAX_DELAY);

  tft.setTextColor(color);
  tft.setTextSize(lightTextSize);
  tft.setCursor(light2OnX, light2OnY);
  tft.print("LIGTH");

  xSemaphoreGive(spi_mutex);
  // CRITICAL SECTION SPI: end
}

// Write the speed in the centre frame
void write_speed(int value) {
  speedLast = _write_int(speedFrameX + 9, speedFrameY + 10, speedLast, value,
                         speedTextSize, ILI9341_WHITE);

  // tft.setFont(&FreeMonoBold24pt7b);
  // speedLast = _write_int(speedFrameX + 20, speedFrameY + 25, speedLast,
  // value,
  //                        2, ILI9341_WHITE);
  // tft.setFont();
}

// acceleration value: 0-255
void write_acceleration(int value) {
  if (value < 0 || value > 999) {
    value = 999;
  }
  accelerationLast = _write_int(accFrameX + 4, accFrameY + 4, accelerationLast,
                                value, accTextSize, ILI9341_GREENYELLOW);
}

void write_bat(float value) {
  int labelOffset = labelLen * batTextSize * 6;
  batLast = _write_float(batFrameX + labelOffset, batFrameY, batLast, value,
                         batTextSize, ILI9341_BLUE);
}

void write_pv(float value) {
  int labelOffset = labelLen * pvTextSize * 6;
  pvLast = _write_float(pvFrameX + labelOffset, pvFrameY, pvLast, value,
                        pvTextSize, ILI9341_YELLOW);
}

void write_motor(float value) {
  int labelOffset = labelLen * motorTextSize * 6;
  motorLast = _write_float(motorFrameX + labelOffset, motorFrameY, motorLast,
                           value, motorTextSize, ILI9341_YELLOW);
}

void _drawCentreString(const String &buf, int x, int y) {
  // CRITICAL SECTION SPI: start
  xSemaphoreTake(spi_mutex, portMAX_DELAY);

  int16_t x1, y1;
  uint16_t w, h;
  tft.getTextBounds(buf, x, y, &x1, &y1, &w, &h); // calc width of new string
  tft.setCursor(x - w / 2, y);
  tft.print(buf);

  xSemaphoreGive(spi_mutex);
  // CRITICAL SECTION SPI: end
}

// INFO = ILI9341_WHITE, STATUS = ILI9341_GREEN, WARN = ILI9341_PURPLE, ERROR =
// ILI9341_RED
int _getColorForInfoType(INFO_TYPE type) {
  int color;
  switch (type) {
  case INFO_TYPE::ERROR:
    color = ILI9341_RED;
    break;

  case INFO_TYPE::WARN:
    color = ILI9341_GREENYELLOW;
    break;

  case INFO_TYPE::STATUS:
    color = ILI9341_GREEN;
    break;

  case INFO_TYPE::INFO:
  default:
    color = ILI9341_WHITE;
    break;
  }
  return color;
}

void write_driver_info(String msg, INFO_TYPE type) {

  // CRITICAL SECTION SPI: start
  xSemaphoreTake(spi_mutex, portMAX_DELAY);

  // int textSize = 1;
  // tft.setFont(&FreeSans18pt7b);

  // if (msg != infoLast) {
  //   tft.setTextSize(textSize);
  //   tft.setTextWrap(true);

  //   tft.setTextColor(bgColor);
  //   tft.setCursor(infoFrameX, infoFrameY + 9);
  //   tft.print(infoLast);

  //   tft.setTextColor(_getColorForInfoType(type));
  //   tft.setCursor(infoFrameX, infoFrameY + 9);
  //   tft.print(msg);
  //   // TODO: _drawCentreString(msg.c_str(), 0, 0);
  //   infoLast = msg;
  // }
  // tft.setFont();

  int textSize = 4;

  if (msg != infoLast) {
    tft.setTextSize(textSize);
    tft.setTextWrap(true);

    tft.setTextColor(bgColor);
    tft.setCursor(infoFrameX, infoFrameY);
    tft.print(infoLast);

    tft.setTextColor(_getColorForInfoType(type));
    tft.setCursor(infoFrameX, infoFrameY);
    tft.print(msg);
    // TODO: _drawCentreString(msg.c_str(), 0, 0);
    infoLast = msg;
  }

  xSemaphoreGive(spi_mutex);
  // CRITICAL SECTION SPI: end
}

void driver_display_demo_screen() {
  printf("Draw demo screen:\n");
#ifdef POWERMEASUREMENT
  // ---- for power measurement: start
  printf(" - black background\n");
  tft.fillScreen(ILI9341_BLACK);
  delay(15000);
  printf(" - white background\n");
  tft.fillScreen(ILI9341_WHITE);
  delay(15000);
  // ---- for power measurement: end
#endif

  printf(" - background\n");
  draw_display_background();
  printf(" - driver info\n");
  write_driver_info("123456789_123456789_123456", INFO_TYPE::INFO);
  printf(" - hazzard warn\n");
  indicator_set_and_blink(INDICATOR::WARN, true);
  printf(" - spped\n");
  write_speed(888);
  printf(" - acceleration\n");
  write_acceleration(888);
  printf(" - increase arrow\n");
  _arrow_increase(ILI9341_YELLOW);
  printf(" - decrease arrow\n");
  _arrow_decrease(ILI9341_RED);
  printf(" - light1 on\n");
  light1OnOff();
  printf(" - light1 on\n");
  light2OnOff();
  printf(" - battery\n");
  write_bat(-8888.8);
  printf(" - photovoltaic\n");
  write_pv(-8888.8);
  printf(" - motor\n");
  write_motor(-8888.8);
  printf(" - life sign\n");
  lifeSign();
  printf("ready.\n");
}

// ------------------
// FreeRTOS INIT TASK
// ------------------
void init_driver_display(void) {
  // CRITICAL SECTION SPI: start
  xSemaphoreTake(spi_mutex, portMAX_DELAY);

#ifdef DISPLAY_OLED_128_6LGA
  // TODO: DISPLAY_OLED_128_6LGA
  println("Display Test DISPLAY_OLED_128_6LGA");
  SPI.begin(4);
  SPI.beginTransaction(SPISettings(1000000, SPI_MSBFIRST, SPI_MODE0));
  SPI.transfer(0xF0);
  SPI.transfer(0x40);
  SPI.transfer(0xA0);
  SPI.transfer(0xC0);
  SPI.transfer(0xA6);

  SPI.transfer(0x81);
  SPI.transfer(0xFF);

  SPI.transfer(0xD5);
  SPI.transfer(0x40);

  SPI.transfer(0xD9);
  SPI.transfer(0x44);

  SPI.transfer(0xAF);
  SPI.endTransaction();
  println("End Display Test DISPLAY_OLED_128_6LGA");
#endif

  tft.begin();
  try {
    uint8_t x = tft.readcommand8(ILI9341_RDMODE);
    printf("Display Power Mode: 0x%x\n", x);
    x = tft.readcommand8(ILI9341_RDMADCTL);
    printf("MADCTL Mode:        0x%x\n", x);
    x = tft.readcommand8(ILI9341_RDPIXFMT);
    printf("Pixel Format:       0x%x\n", x);
    x = tft.readcommand8(ILI9341_RDIMGFMT);
    printf("Image Format:       0x%x\n", x);
    x = tft.readcommand8(ILI9341_RDSELFDIAG);
    printf("Self Diagnostic:    0x%x\n", x);
    infoFrameSizeX = tft.width();
    speedFrameX = (tft.width() - speedFrameSizeX) / 2;
    printf("[Display ILI9341] Initialize successfully with screen %d x %d.\n",
           tft.height(), tft.width());
  } catch (__exception ex) {
    printf("[Display ILI9341] Unable to initialize screen.\n");
  }

  xSemaphoreGive(spi_mutex);
  // CRITICAL SECTION SPI: end

  // tft.setFont(&FreeSans9pt7b);
  driver_display_demo_screen();
  delay(1000);
  write_driver_info("ready.", INFO_TYPE::INFO);
  indicator_set_and_blink(INDICATOR::OFF, false);
  write_speed(0);
  write_acceleration(0);
  _arrow_increase(bgColor);
  _arrow_decrease(bgColor);
  _light1(false);
  _light2(false);
  write_bat(0.0);
  write_pv(0.0);
  write_motor(0.0);

  printf("[v] Driver Display inited: SPI_CS_TFT=%d, SPI_DC=%d, SPI_MOSI=%d, "
         "SPI_CLK=%d, SPI_RST=%d, SPI_MISO=%d.\n",
         SPI_CS_TFT, SPI_DC, SPI_MOSI, SPI_CLK, SPI_RST, SPI_MISO);
  vTaskDelay(500 / portTICK_PERIOD_MS);
}

// -------------
// FreeRTOS TASK
// -------------
void driver_display_task(void *pvParameter) {
  // polling loop
  while (1) {
    // long start = micros();

    // // CRITICAL SECTION SPI: start
    // xSemaphoreTake(spi_mutex, portMAX_DELAY);
    // xSemaphoreGive(spi_mutex);
    // // CRITICAL SECTION SPI: end

    if (lifeSignCounter > 10) {
      lifeSign();
      lifeSignCounter = 0;
    }
    lifeSignCounter++;

    // printf("time elapsed: %ld\n", micros() - start);

    // sleep for 1s
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}
