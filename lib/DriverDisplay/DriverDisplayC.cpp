/***
 *
 * Driver's Display
 *
 * The driver's display is passive with the exception of the indicator function.
 * This means that all sensors send their values to the display.
 *
 ***/

#include <LocalFunctionsAndDevices.h>

#include <abstract_task.h>
#include <definitions.h>

#include "ADC.h"
#include "DriverDisplayC.h"

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

extern SPIBus spiBus;
extern ADC adc;
extern bool systemOk;

Adafruit_ILI9341 tft = Adafruit_ILI9341(0, 0, 0, 0, 0, 0);
// namespace DriverDisplayC {

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
CONSTANT_MODE constant_drive_mode = CONSTANT_MODE::SPEED;
bool constant_drive_set = false;
//=======================================

DriverDisplayC *DriverDisplayC::_instance = 0;
DriverDisplayC::DISPLAY_STATUS DriverDisplayC::status = DISPLAY_STATUS::SETUP;

string DriverDisplayC ::getName() { return "Display0 (driver display)"; };

void DriverDisplayC ::init() {
  abstract_task::init();
  re_init();
  status = DISPLAY_STATUS::DISPLAY_DEMOSCREEN;
}

void DriverDisplayC ::re_init(void) {
  _setup();
  status = DISPLAY_STATUS::DISPLAY_BACKGROUND;
}

void DriverDisplayC ::_setup() {
  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
  tft = Adafruit_ILI9341(SPI_CS_TFT, SPI_DC, SPI_MOSI, SPI_CLK, SPI_RST, SPI_MISO);
  tft.begin();
  printf("done.\n");
  try {
    printf("      Display0 (driver display) initializing...\n");
    uint8_t x = tft.readcommand8(ILI9341_RDMODE);
    printf("      Display Power Mode: 0x%x\n", x);
    x = tft.readcommand8(ILI9341_RDMADCTL);
    printf("      MADCTL Mode:        0x%x\n", x);
    x = tft.readcommand8(ILI9341_RDPIXFMT);
    printf("      Pixel Format:       0x%x\n", x);
    x = tft.readcommand8(ILI9341_RDIMGFMT);
    printf("      Image Format:       0x%x\n", x);
    x = tft.readcommand8(ILI9341_RDSELFDIAG);
    printf("      Self Diagnostic:    0x%x\n", x);
    infoFrameSizeX = tft.width();
    speedFrameX = (tft.width() - speedFrameSizeX) / 2;
    printf("[v] Display0 (driver display) inited: screen ILI9341 with %d x %d.\n", tft.height(), tft.width());
  } catch (__exception ex) {
    printf("[x] Display0 (driver display): Unable to initialize screen ILI9341.\n");
  }
  xSemaphoreGive(spiBus.mutex);
}

void DriverDisplayC ::exit() {}

// -------------
// FreeRTOS TASK
// -------------
void DriverDisplayC ::task(void) {
  // polling loop
  while (1) {
    switch (status) {
    case DISPLAY_STATUS::DISPLAY_DEMOSCREEN:
      draw_display_background();
      driver_display_demo_screen();
      status = DISPLAY_STATUS::DISPLAY_BACKGROUND;
      break;
    case DISPLAY_STATUS::DISPLAY_BACKGROUND:
      // draw_display_background();

      status = DISPLAY_STATUS::WORK;
      break;
    default:
      if (lifeSignCounter > 10) {
        lifeSign();
        lifeSignCounter = 0;
        int accDisplayValue = adc.read_adc_acc_dec();
        write_acceleration(accDisplayValue);
        //speedCheck(speedLast);
      }
      lifeSignCounter++;
      break;
    }
    // sleep for sleep_polling_ms
    this->sleep(20);
  }
}

void DriverDisplayC::speedCheck(int speed) {
  DriverDisplayC *dd = DriverDisplayC::instance();
  if (speed < 50) {
    dd->arrow_increase(true);
  } else {
    dd->arrow_increase(false);
  }
  if (speed > 80) {
    dd->arrow_decrease(true);
  } else {
    dd->arrow_decrease(false);
  }
}

void exit(void) {}

// writes flDriverDisplayC ::oat value  in the range from -9999.9 to 9999.9
float DriverDisplayC ::_write_float(int x, int y, float valueLast, float value, int textSize, int color) {

  if (value < -9999.9 || value > 9999.9) {
    printf("ERROR: call _write_float with a value outside the range: '%f'\n", value);
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
  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);

  tft.setTextSize(textSize);
  tft.setTextColor(color);
  tft.setCursor(x, y);
  // if a change in the digit then replace the old with new value by
  // first deleting the digit area and second write the new value
  if (d0 != d0o) { // || d0o == 0) {
    tft.fillRect(x + (digitWidth + 1) * 5, y, digitWidth * 2, digitHeight, bgColor);
    tft.setCursor(x + (digitWidth + 1) * 5, y);
    tft.printf(".%d", d0);
  }
  if (d1 != d1o || d1o == 0) {
    tft.fillRect(x + (digitWidth + 1) * 4, y, digitWidth, digitHeight, bgColor);
    tft.setCursor(x + (digitWidth + 1) * 4, y);
    tft.print(d1);
  }
  if (d2 != d2o || d2o == 0) {
    tft.fillRect(x + (digitWidth + 1) * 3, y, digitWidth, digitHeight, bgColor);
    tft.setCursor(x + (digitWidth + 1) * 3, y);
    if (abs(value) > 9) {
      tft.print(d2);
    }
  }
  if (d3 != d3o || d3o == 0) {
    tft.fillRect(x + (digitWidth + 1) * 2, y, digitWidth, digitHeight, bgColor);
    tft.setCursor(x + (digitWidth + 1) * 2, y);
    if (abs(value) > 99) {
      tft.print(d3);
    }
  }
  if (d4 != d4o || d4o == 0) {
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

  xSemaphoreGive(spiBus.mutex);
  // CRITICAL SECTION SPI: end

  return value;
}

// writes integer value in the range from -99 to +99
int DriverDisplayC ::_write_ganz_99(int x, int y, int valueLast, int value, int textSize, int color) {
  if (value < -99 || value > 999) {
    printf("ERROR: call _write_ganz_99 with a value outside the range: '%d'", value);
    return value;
  }
  // determine the sign of new and old value
  char sign = value < 0 ? '-' : '+';
  char signOld = valueLast < 0 ? '-' : '+';
  // determine the four digits and one decimal of the new value
  int val = abs(value);

  // determine the three digits the stored, new value
  int d1 = (int)val % 10;
  int d2 = ((int)val / 10) % 10;
  // determine the three digits the stored, old value
  int valLast = abs(valueLast);
  int d1o = (int)valLast % 10;
  int d2o = ((int)valLast / 10) % 10;

  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);

  tft.setTextSize(textSize);
  tft.setTextColor(color);
  tft.setCursor(x, y);
  int digitWidth = textSize * 6;
  int digitHeight = textSize * 8;
  // if a change in the digit then replace the old with new value by
  // first deleting the digit area and second write the new value
  if (d1 != d1o) { //|| d1o == 0) {
    tft.fillRect(x + (digitWidth + 1) * 2, y, digitWidth, digitHeight, bgColor);
    tft.setCursor(x + (digitWidth + 1) * 2, y);
    tft.print(d1);
  }
  if (d2 != d2o || d2o == 0) {
    tft.fillRect(x + (digitWidth + 1) * 1, y, digitWidth, digitHeight, bgColor);
    tft.setCursor(x + (digitWidth + 1) * 1, y);
    if (abs(value) > 9) {
      tft.print(d2);
    }
  }
  if (sign != signOld) {
    tft.fillRect(x, y, (digitWidth + 1), digitHeight, bgColor);
    tft.setCursor(x, y);
    // if (sign == '-') {
    tft.print(sign);
    //}
  }
  xSemaphoreGive(spiBus.mutex);

  return value;
}

// writes integer value in the range from 0 to 999
int DriverDisplayC ::_write_nat_999(int x, int y, int valueLast, int value, int textSize, int color) {
  if (value < 0 || value > 999) {
    printf("ERROR: call _write_nat_999 with a value outside the range: '%d'", value);
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

  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);

  tft.setTextSize(textSize);
  tft.setTextColor(color);
  tft.setCursor(x, y);
  // if a change in the digit then replace the old with new value by
  // first deleting the digit area and second write the new value
  if (d1 != d1o) { //} || d1o == 0) {
    tft.fillRect(x + (digitWidth + 1) * 2, y, digitWidth, digitHeight, bgColor);
    tft.setCursor(x + (digitWidth + 1) * 2, y);
    tft.print(d1);
  }
  if (d2 != d2o || d2o == 0) {
    tft.fillRect(x + (digitWidth + 1) * 1, y, digitWidth, digitHeight, bgColor);
    tft.setCursor(x + (digitWidth + 1) * 1, y);
    if (abs(value) > 9) {
      tft.print(d2);
    }
  }
  if (d3 != d3o || d3o == 0) {
    tft.fillRect(x + (digitWidth + 1) * 0, y, digitWidth, digitHeight, bgColor);
    tft.setCursor(x + (digitWidth + 1) * 0, y);
    if (abs(value) > 99) {
      tft.print(d3);
    }
  }

  xSemaphoreGive(spiBus.mutex);

  return value;
}

// write color of the border of the main display
void DriverDisplayC ::draw_display_border(int color) {
  // CRITICAL SECTION SPI: start
  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);

  tft.drawRoundRect(0, mainFrameX, tft.width(), tft.height() - mainFrameX, 8, color);

  xSemaphoreGive(spiBus.mutex);
  // CRITICAL SECTION SPI: end
}

// write color of the border of the speed display
void DriverDisplayC ::draw_speed_border(int color) {
  // CRITICAL SECTION SPI: start
  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);

  tft.drawRoundRect(speedFrameX, speedFrameY, speedFrameSizeX, speedFrameSizeY, 4, color);

  xSemaphoreGive(spiBus.mutex);
  // CRITICAL SECTION SPI: end
}

// write color of the border of the speed display
void DriverDisplayC ::draw_acceleration_border(int color) {
  // CRITICAL SECTION SPI: start
  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);

  accFrameSizeX = speedFrameX - 3;
  tft.drawRoundRect(accFrameX, accFrameY, accFrameSizeX, accFrameSizeY, 4, color);

  xSemaphoreGive(spiBus.mutex);
  // CRITICAL SECTION SPI: end
}

void DriverDisplayC ::lifeSign() {
  int color;
  if (lifeSignState) {
    color = bgColor;
  } else {
    color = ILI9341_GREEN;
  }

  // CRITICAL SECTION SPI: start
  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);

  lifeSignX = tft.width() - lifeSignRadius - 6;
  lifeSignY = tft.height() - lifeSignRadius - 6;
  tft.fillCircle(lifeSignX, lifeSignY, lifeSignRadius, color);

  xSemaphoreGive(spiBus.mutex);
  // CRITICAL SECTION SPI: end

  lifeSignState = !lifeSignState;
}

void DriverDisplayC ::draw_display_background() {
  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);

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

  xSemaphoreGive(spiBus.mutex);

  infoFrameSizeX = tft.width();
  speedFrameX = (tft.width() - speedFrameSizeX) / 2;

  draw_display_border(ILI9341_GREEN);
  draw_speed_border(ILI9341_YELLOW);
  draw_acceleration_border(ILI9341_YELLOW);

  lifeSignCounter = 0;
  lifeSignState = false;

  String infoLast = "";
  write_driver_info(infoLast = "", INFO_TYPE::STATUS);

  speedLast = -1;
  write_speed(0);

  accelerationLast = -1;
  write_acceleration(0);

  batLast = -1;
  write_bat(0);

  pvLast = -1;
  write_pv(0);

  motorLast = -1;
  write_motor(0);

  light1On = false;
  light2On = false;
}

void DriverDisplayC ::_arrow_increase(int color) {
  int x = speedFrameX;
  int y = speedFrameY - 3;

  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
  tft.fillTriangle(x, y, x + speedFrameSizeX, y, x + speedFrameSizeX / 2, y - 10, color);
  xSemaphoreGive(spiBus.mutex);
}

void DriverDisplayC ::_arrow_decrease(int color) {
  int x = speedFrameX;
  int y = speedFrameY + speedFrameSizeY + 3;

  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
  tft.fillTriangle(x, y, x + speedFrameSizeX, y, x + speedFrameSizeX / 2, y + 10, color);
  xSemaphoreGive(spiBus.mutex);
}

// show the slower arrow (red under the speed display)
void DriverDisplayC ::arrow_decrease(bool on) {
  int color = bgColor;
  if (on) {
    arrow_increase(false);
    color = ILI9341_RED;
  }
  _arrow_decrease(color);
}

// show the faster arrow (green above the speed display)
void DriverDisplayC ::arrow_increase(bool on) {
  int color = bgColor;
  if (on) {
    arrow_decrease(false);
    color = ILI9341_YELLOW;
  }
  _arrow_increase(color);
}

void DriverDisplayC ::constant_drive_mode_set(CONSTANT_MODE mode) {
  constant_drive_mode = mode;
  if (constant_drive_set == true) {
    constant_drive_mode_show();
  }
}

void DriverDisplayC ::constant_drive_on() {
  constant_drive_set = true;
  constant_drive_mode_show();
}

void DriverDisplayC ::constant_drive_off() {
  constant_drive_set = false;
  constant_drive_mode_hide();
}

void DriverDisplayC ::constant_drive_mode_hide(){
  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);

  tft.setTextSize(constantModeTextSize);
  tft.setTextColor(ILI9341_BLACK);
  tft.setCursor(constantModeX, constantModeY);
  tft.print("power");
  tft.setCursor(constantModeX, constantModeY);
  tft.print("speed");
  // tft.writeFillRect(constantModeX, constantModeY, constantModeTextSize * 5, constantModeTextSize * 1, ILI9341_BLACK);
    xSemaphoreGive(spiBus.mutex);
}

void DriverDisplayC ::constant_drive_mode_show() {

  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);

  tft.setTextSize(constantModeTextSize);
  tft.setTextColor(ILI9341_BLACK);
  tft.setCursor(constantModeX, constantModeY);
  tft.print("power");
  tft.setCursor(constantModeX, constantModeY);
  tft.print("speed");
  // tft.writeFillRect(constantModeX, constantModeY, constantModeTextSize * 5, constantModeTextSize * 1, ILI9341_BLACK);

  tft.setTextColor(ILI9341_YELLOW);
  tft.setCursor(constantModeX, constantModeY);
  if (constant_drive_mode == CONSTANT_MODE::POWER) {
    tft.print("power");
  } else {
    tft.print("speed");
  }

  xSemaphoreGive(spiBus.mutex);
}

void DriverDisplayC ::write_drive_direction(DRIVE_DIRECTION direction) {
  // CRITICAL SECTION SPI: start
  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);

  tft.setTextSize(driveDirectionTextSize);
  tft.setTextColor(ILI9341_BLACK);
  tft.setCursor(driveDirectionX, driveDirectionY);
  tft.print("forward");
  tft.setCursor(driveDirectionX, driveDirectionY);
  tft.print("backward");

  tft.setCursor(driveDirectionX, driveDirectionY);
  if (direction == DRIVE_DIRECTION::FORWARD) {
    tft.setTextColor(ILI9341_YELLOW);
    tft.print("forward");
  } else {
    tft.setTextColor(ILI9341_RED);
    tft.print("backward");
  }
  xSemaphoreGive(spiBus.mutex);
  // CRITICAL SECTION SPI: end
}

void DriverDisplayC ::_turn_Left(int color) {
  int x = indicatorLeftX;
  int y = indicatorY;

  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
  tft.fillTriangle(x, y, x + indicatorWidth, y - indicatorHeight, x + indicatorWidth, y + indicatorHeight, color);
  xSemaphoreGive(spiBus.mutex);
}

void DriverDisplayC ::_turn_Right(int color) {
  int x = indicatorRightX;
  int y = indicatorY;

  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
  tft.fillTriangle(x, y, x - indicatorWidth, y - indicatorHeight, x - indicatorWidth, y + indicatorHeight, color);
  xSemaphoreGive(spiBus.mutex);
}

void DriverDisplayC ::indicator_set_and_blink(INDICATOR direction) { indicator_set_and_blink(direction, true); }

void DriverDisplayC ::indicator_set_and_blink(INDICATOR direction, bool blinkOn) {
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
}

void DriverDisplayC ::_light1(bool lightOn) {
  light1On = !lightOn;
  light1OnOff();
}

void DriverDisplayC ::_light2(bool lightOn) {
  light2On = !lightOn;
  light2OnOff();
}

void DriverDisplayC ::light1OnOff() {
  int color = ILI9341_YELLOW;
  if (light1On) {
    color = bgColor;
    _light2(false);
  }

  light1On = !light1On;

  // CRITICAL SECTION SPI: start
  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);

  tft.setTextColor(color);
  tft.setTextSize(lightTextSize);
  tft.setCursor(light1OnX, light1OnY);
  tft.print("Light");

  xSemaphoreGive(spiBus.mutex);
  // CRITICAL SECTION SPI: end
}

void DriverDisplayC ::light2OnOff() {
  int color = ILI9341_BLUE;
  if (light2On) {
    color = bgColor;
  } else {
    _light1(true);
  }
  light2On = !light2On;

  // CRITICAL SECTION SPI: start
  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);

  tft.setTextColor(color);
  tft.setTextSize(lightTextSize);
  tft.setCursor(light2OnX, light2OnY);
  tft.print("LIGTH");

  xSemaphoreGive(spiBus.mutex);
  // CRITICAL SECTION SPI: end
}

// Write the speed in the centre frame
void DriverDisplayC ::write_speed(int value) {
  speedLast = _write_nat_999(speedFrameX + 9, speedFrameY + 10, speedLast, value, speedTextSize, ILI9341_WHITE);

  // tft.setFont(&FreeMonoBold24pt7b);
  // speedLast = _write_nat_999(speedFrameX + 20, speedFrameY + 25, speedLast,
  // value, 2, ILI9341_WHITE);
  // tft.setFont();
}

// acceleration value: 0-255
void DriverDisplayC ::write_acceleration(int value) {
  if (value < -99 || value > 99) {
    value = 999;
  }
  accelerationLast = _write_ganz_99(accFrameX + 4, accFrameY + 4, accelerationLast, value, accTextSize, ILI9341_GREENYELLOW);
}

void DriverDisplayC ::write_bat(float value) {
  int labelOffset = labelLen * batTextSize * 6;
  batLast = _write_float(batFrameX + labelOffset, batFrameY, batLast, value, batTextSize, ILI9341_BLUE);
}

void DriverDisplayC ::write_pv(float value) {
  int labelOffset = labelLen * pvTextSize * 6;
  pvLast = _write_float(pvFrameX + labelOffset, pvFrameY, pvLast, value, pvTextSize, ILI9341_YELLOW);
}

void DriverDisplayC ::write_motor(float value) {
  int labelOffset = labelLen * motorTextSize * 6;
  motorLast = _write_float(motorFrameX + labelOffset, motorFrameY, motorLast, value, motorTextSize, ILI9341_YELLOW);
}

void DriverDisplayC ::_drawCentreString(const String &buf, int x, int y) {
  // CRITICAL SECTION SPI: start
  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);

  int16_t x1, y1;
  uint16_t w, h;
  tft.getTextBounds(buf, x, y, &x1, &y1, &w, &h); // calc width of new string
  tft.setCursor(x - w / 2, y);
  tft.print(buf);

  xSemaphoreGive(spiBus.mutex);
  // CRITICAL SECTION SPI: end
}

// INFO = ILI9341_WHITE, STATUS = ILI9341_GREEN, WARN = ILI9341_PURPLE, ERROR
// = ILI9341_RED
int DriverDisplayC ::_getColorForInfoType(INFO_TYPE type) {
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

void DriverDisplayC ::write_driver_info(String msg, INFO_TYPE type) {
  // comments are preparation for font usage

  // CRITICAL SECTION SPI: start
  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);

  int textSize = 4;
  // int textSize = 1;
  // tft.setFont(&FreeSans18pt7b);

  if (msg != infoLast) {
    tft.setTextSize(textSize);
    tft.setTextWrap(true);

    tft.setTextColor(bgColor);
    // tft.setCursor(infoFrameX, infoFrameY + 9);
    tft.setCursor(infoFrameX, infoFrameY);
    tft.print(infoLast);

    tft.setTextColor(_getColorForInfoType(type));
    tft.setCursor(infoFrameX, infoFrameY);
    tft.print(msg);
    // TODO: _drawCentreString(msg.c_str(), 0, 0);
    infoLast = msg;
  }
  // tft.setFont();

  xSemaphoreGive(spiBus.mutex);
  // CRITICAL SECTION SPI: end
}

void DriverDisplayC ::driver_display_demo_screen() {
  printf("  Draw demo screen:\n");
#ifdef POWERMEASUREMENT
  // ---- for power measurement: start
  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
  printf(" - black background\n");
  tft.fillScreen(ILI9341_BLACK);
  delay(15000);
  printf(" - white background\n");
  tft.fillScreen(ILI9341_WHITE);
  delay(15000);
  xSemaphoreGive(spiBus.mutex);
  // ---- for power measurement: end
#endif
  printf("   - driver info\n");
  write_driver_info("123456789_123456789_123456", INFO_TYPE::INFO);
  printf("   - hazzard warn\n");
  indicator_set_and_blink(INDICATOR::WARN, true);
  printf("   - spped\n");
  write_speed(888);
  printf("   - acceleration\n");
  write_acceleration(888);
  printf("   - increase arrow\n");
  arrow_increase(true);
  printf("   - decrease arrow\n");
  arrow_decrease(true);
  printf("   - light1 on\n");
  light1OnOff();
  printf("   - light1 on\n");
  light2OnOff();
  printf("   - constant mode speed\n");
  constant_drive_mode_set(CONSTANT_MODE::SPEED);
  constant_drive_mode_show();
  printf("   - drive direction forwards\n");
  write_drive_direction(DRIVE_DIRECTION::FORWARD);
  printf("   - battery\n");
  write_bat(-8888.8);
  printf("   - photovoltaic\n");
  write_pv(-8888.8);
  printf("   - motor\n");
  write_motor(-8888.8);
  printf("   - constant mode power\n");
  constant_drive_mode_set(CONSTANT_MODE::POWER);
  constant_drive_mode_show();
  printf("   - drive direction backwards\n");
  write_drive_direction(DRIVE_DIRECTION::BACKWARD);
  printf("   - life sign\n");
  lifeSign();

  // set default initial dosplay values
  write_driver_info("", INFO_TYPE::INFO);
  indicator_set_and_blink(INDICATOR::WARN, false);
  write_speed(0);
  write_acceleration(0);
  arrow_increase(false);
  arrow_decrease(false);
  light1On = true;
  light1OnOff();
  light2On = true;
  light2OnOff();
  constant_drive_mode_set(CONSTANT_MODE::SPEED);
  constant_drive_mode_hide();
  write_bat(0.0);
  write_pv(0.0);
  write_motor(0.0);
  write_drive_direction(DRIVE_DIRECTION::FORWARD);

  printf("  End of demo screen.\n");
}

// } //namespace DriverDisplayC
