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

#include <ADC.h>
#include <Display.h>
#include <DriverDisplay.h>

#include <CarState.h>
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
extern CarState carState;
extern Adafruit_ILI9341 tft;

// namespace Display {

//==== display cache =====================
// ... to avoid flickering
int speedLast = -1;
int accelerationLast = -1;
float batLast = -1;
float pvLast = -1;
float motorLast = -1;
//=======================================

DISPLAY_STATUS DriverDisplay::display_setup(DISPLAY_STATUS status) {
  printf("[?] Setup 'DriverDisplay'...\n");
  int height = 0;
  int width = 0;
  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
  try {
    height = tft.height();
    width = tft.width();
    tft.setRotation(1);
    tft.setTextSize(1);
    tft.setTextColor(ILI9341_BLUE);
    tft.setScrollMargins(10, width - 20);
    infoFrameSizeX = width;
    speedFrameX = (width - speedFrameSizeX) / 2;
  } catch (__exception ex) {
    xSemaphoreGive(spiBus.mutex);
    printf("[x] DriverDisplay: Unable to initialize screen ILI9341.\n");
    throw ex;
  }
  xSemaphoreGive(spiBus.mutex);
  printf("[v] DriverDisplay inited: screen ILI9341 with %d x %d.\n", height, width);
  return DISPLAY_STATUS::CONSOLE;
}

void DriverDisplay::print(string msg) {
  if (get_DisplayStatus() == DISPLAY_STATUS::CONSOLE) {
    xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
    tft.setTextSize(1);
    tft.print(msg.c_str());
    xSemaphoreGive(spiBus.mutex);
  } else {
    carState.DriverInfo.set(msg);
    carState.DriverInfoType.set(INFO_TYPE::INFO);
    write_driver_info();
  }
}

void DriverDisplay::speedCheck(int speed) {
  if (speed < 50) {
    arrow_increase(true);
  } else {
    arrow_increase(false);
  }
  if (speed > 80) {
    arrow_decrease(true);
  } else {
    arrow_decrease(false);
  }
}

// writes DriverDisplay::float value  in the range from -9999.9 to 9999.9
// float DriverDisplay::_write_float(int x, int y, float valueLast, float value, int textSize, int color) {

//   if (value < -9999.9 || value > 9999.9) {
//     printf("ERROR: call _write_float with a value outside the range: '%f'\n", value);
//     return value;
//   }
//   int digitWidth = textSize * 6;
//   int digitHeight = textSize * 8;
//   // determine the sign of new and old value
//   char sign = value < 0 ? '-' : '+';
//   char signOld = valueLast < 0 ? '-' : '+';
//   // determine the four digits and one decimal of the new value
//   float val = abs(value);
//   int d1 = (int)val % 10;
//   int d2 = ((int)val / 10) % 10;
//   int d3 = ((int)val / 100) % 10;
//   int d4 = ((int)val / 1000) % 10;
//   int d0 = (val - (int)val) * 10;
//   // determine the four digits and one decimal of the stored, old value
//   float valOld = abs(valueLast);
//   int d1o = (int)valOld % 10;
//   int d2o = ((int)valOld / 10) % 10;
//   int d3o = ((int)valOld / 100) % 10;
//   int d4o = ((int)valOld / 1000) % 10;
//   int d0o = (valOld - (int)valOld) * 10;

//   xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
//   tft.setTextSize(textSize);
//   tft.setTextColor(color);
//   tft.setCursor(x, y);
//   // if a change in the digit then replace the old with new value by
//   // first deleting the digit area and second write the new value
//   if (d0 != d0o) { // || d0o == 0) {
//     tft.fillRect(x + (digitWidth + 1) * 5, y, digitWidth * 2, digitHeight, bgColor);
//     tft.setCursor(x + (digitWidth + 1) * 5, y);
//     tft.printf(".%d", d0);
//   }
//   if (d1 != d1o || d1o == 0) {
//     tft.fillRect(x + (digitWidth + 1) * 4, y, digitWidth, digitHeight, bgColor);
//     tft.setCursor(x + (digitWidth + 1) * 4, y);
//     tft.print(d1);
//   }
//   if (d2 != d2o || d2o == 0) {
//     tft.fillRect(x + (digitWidth + 1) * 3, y, digitWidth, digitHeight, bgColor);
//     tft.setCursor(x + (digitWidth + 1) * 3, y);
//     if (abs(value) > 9) {
//       tft.print(d2);
//     }
//   }
//   if (d3 != d3o || d3o == 0) {
//     tft.fillRect(x + (digitWidth + 1) * 2, y, digitWidth, digitHeight, bgColor);
//     tft.setCursor(x + (digitWidth + 1) * 2, y);
//     if (abs(value) > 99) {
//       tft.print(d3);
//     }
//   }
//   if (d4 != d4o || d4o == 0) {
//     tft.fillRect(x + (digitWidth + 1) * 1, y, digitWidth, digitHeight, bgColor);
//     tft.setCursor(x + (digitWidth + 1) * 1, y);
//     if (abs(value) > 999) {
//       tft.print(d4);
//     }
//   }
//   if (sign != signOld) {
//     tft.fillRect(x, y, (digitWidth + 1), digitHeight, bgColor);
//     tft.setCursor(x, y);
//     tft.print(sign);
//   }
//   tft.setTextSize(1);
//   xSemaphoreGive(spiBus.mutex);

//   return value;
// }

// writes integer value in the range from -99 to +99
int DriverDisplay::_write_ganz_99(int x, int y, int valueLast, int value, int textSize, int color) {
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

  int digitWidth = textSize * 6;
  int digitHeight = textSize * 8;
  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
  tft.setTextSize(textSize);
  tft.setTextColor(color);
  tft.setCursor(x, y);
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
  tft.setTextSize(1);
  xSemaphoreGive(spiBus.mutex);

  return value;
}

// writes integer value in the range from 0 to 999
int DriverDisplay::_write_nat_999(int x, int y, int valueLast, int value, int textSize, int color) {
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
  tft.setTextSize(1);
  xSemaphoreGive(spiBus.mutex);

  return value;
}

// write color of the border of the main display
void DriverDisplay::draw_display_border(int color) {
  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
  tft.drawRoundRect(0, mainFrameX, tft.width(), tft.height() - mainFrameX, 8, color);
  xSemaphoreGive(spiBus.mutex);
}

// write color of the border of the speed display
void DriverDisplay::draw_speed_border(int color) {
  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
  tft.drawRoundRect(speedFrameX, speedFrameY, speedFrameSizeX, speedFrameSizeY, 4, color);
  xSemaphoreGive(spiBus.mutex);
}

// write color of the border of the speed display
void DriverDisplay::draw_acceleration_border(int color) {
  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
  accFrameSizeX = speedFrameX - 3;
  tft.drawRoundRect(accFrameX, accFrameY, accFrameSizeX, accFrameSizeY, 4, color);
  xSemaphoreGive(spiBus.mutex);
}

void DriverDisplay::draw_display_background() {
  infoFrameSizeX = tft.width();
  speedFrameX = (tft.width() - speedFrameSizeX) / 2;

  draw_display_border(ILI9341_GREEN);
  draw_speed_border(ILI9341_YELLOW);
  draw_acceleration_border(ILI9341_YELLOW);
}

void DriverDisplay::_arrow_increase(int color) {
  int x = speedFrameX;
  int y = speedFrameY - 3;

  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
  tft.fillTriangle(x, y, x + speedFrameSizeX, y, x + speedFrameSizeX / 2, y - 10, color);
  xSemaphoreGive(spiBus.mutex);
}

void DriverDisplay::_arrow_decrease(int color) {
  int x = speedFrameX;
  int y = speedFrameY + speedFrameSizeY + 3;

  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
  tft.fillTriangle(x, y, x + speedFrameSizeX, y, x + speedFrameSizeX / 2, y + 10, color);
  xSemaphoreGive(spiBus.mutex);
}

// show the slower arrow (red under the speed display)
void DriverDisplay::arrow_decrease(bool on) {
  int color = bgColor;
  if (on) {
    arrow_increase(false);
    color = ILI9341_RED;
  }
  _arrow_decrease(color);
}

// show the faster arrow (green above the speed display)
void DriverDisplay::arrow_increase(bool on) {
  int color = bgColor;
  if (on) {
    arrow_decrease(false);
    color = ILI9341_YELLOW;
  }
  _arrow_increase(color);
}

#define SPEED_STRING "Speed"
#define POWER_STRING "Power"

void DriverDisplay::constant_drive_mode_show() {
  int width = getPixelWidthOfTexts(constantModeTextSize, SPEED_STRING, POWER_STRING) + 4;
  if (carState.ConstantMode.get() == CONSTANT_MODE::NONE || !carState.ConstantModeOn.get()) {
    xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
    tft.fillRoundRect(constantModeX - 2, constantModeY - 2, width, 18, 3, ILI9341_BLACK);
    xSemaphoreGive(spiBus.mutex);
    return;
  }

  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
  tft.setCursor(constantModeX, constantModeY);
  tft.fillRoundRect(constantModeX - 2, constantModeY - 2, width, 18, 3, ILI9341_YELLOW);
  tft.setTextSize(constantModeTextSize);
  tft.setTextColor(ILI9341_BLACK);
  if (carState.ConstantMode.get() == CONSTANT_MODE::POWER) {
    tft.print(POWER_STRING);
  } else {
    tft.print(SPEED_STRING);
  }
  tft.setTextSize(1);
  xSemaphoreGive(spiBus.mutex);
}

#define FORWARD_STRING ""
#define BACKWARD_STRING "Backward"

void DriverDisplay::write_drive_direction() {
  DRIVE_DIRECTION direction = carState.DriveDirection.get();
  int width = getPixelWidthOfTexts(driveDirectionTextSize, FORWARD_STRING, BACKWARD_STRING) + 4;

  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
  tft.fillRoundRect(driveDirectionX - 2, driveDirectionY - 2, width, 18, 3, ILI9341_BLACK);
  tft.setTextSize(driveDirectionTextSize);
  tft.setCursor(driveDirectionX, driveDirectionY);
  if (carState.DriveDirection.get() == DRIVE_DIRECTION::FORWARD) {
    tft.setTextColor(ILI9341_YELLOW);
    tft.print(FORWARD_STRING);
  } else {
    tft.setTextColor(ILI9341_RED);
    tft.print(BACKWARD_STRING);
  }
  tft.setTextSize(1);
  xSemaphoreGive(spiBus.mutex);
}

void DriverDisplay::_turn_Left(int color) {
  int x = indicatorLeftX;
  int y = indicatorY;

  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
  tft.fillTriangle(x, y, x + indicatorWidth, y - indicatorHeight, x + indicatorWidth, y + indicatorHeight, color);
  xSemaphoreGive(spiBus.mutex);
}

void DriverDisplay::_turn_Right(int color) {
  int x = indicatorRightX;
  int y = indicatorY;

  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
  tft.fillTriangle(x, y, x - indicatorWidth, y - indicatorHeight, x - indicatorWidth, y + indicatorHeight, color);
  xSemaphoreGive(spiBus.mutex);
}

void DriverDisplay::indicator_set_and_blink(INDICATOR direction) { indicator_set_and_blink(direction, true); }

void DriverDisplay::indicator_set_and_blink(INDICATOR direction, bool blinkOn) {
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

#define LIGHT1_STRING "Light"
#define LIGHT2_STRING "LIGHT"

void DriverDisplay::_hide_light() {
  int width = getPixelWidthOfTexts(lightTextSize, LIGHT1_STRING, LIGHT2_STRING) + 4;
  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
  tft.fillRoundRect(lightX - 2, lightY - 2, width, 18, 3, ILI9341_BLACK);
  xSemaphoreGive(spiBus.mutex);
}

void DriverDisplay::show_light() {
  if (carState.Light.get() == LIGHT::OFF) {
    _hide_light();
    return;
  }
  int width = getPixelWidthOfTexts(lightTextSize, LIGHT1_STRING, LIGHT2_STRING) + 4;
  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
  tft.setCursor(lightX, lightY);
  tft.setTextSize(lightTextSize);
  tft.fillRoundRect(lightX - 2, lightY - 2, width, 18, 3, ILI9341_BLACK);
  if (carState.Light.get() == LIGHT::L1) {
    tft.setTextColor(ILI9341_YELLOW);
    tft.print(LIGHT1_STRING);
  } else {
    tft.setTextColor(ILI9341_BLUE);
    tft.print(LIGHT2_STRING);
  }
  tft.setTextSize(1);
  xSemaphoreGive(spiBus.mutex);
}

// Write the speed in the centre frame: 0...999
void DriverDisplay::write_speed() {
  int value = carState.Speed.get();
  if (value < 0 || value > 999) {
    value = 999;
  }
  speedLast = _write_nat_999(speedFrameX + 9, speedFrameY + 10, speedLast, value, speedTextSize, ILI9341_WHITE);
}

// acceleration value: -99...+99
void DriverDisplay::write_acceleration() {
  int value = carState.AccelerationDisplay.get();
  if (value > -99 && value < 99) {
    accelerationLast = _write_ganz_99(accFrameX + 4, accFrameY + 4, accelerationLast, value, accTextSize, ILI9341_GREENYELLOW);
  }
}

void DriverDisplay::_drawCentreString(const string &buf, int x, int y) {
  // // CRITICAL SECTION SPI: start
  // xSemaphoreTake(spiBus.mutex, portMAX_DELAY);

  // int16_t x1, y1;
  // uint16_t w, h;
  // tft.getTextBounds(buf, &x, &y, &x1, &y1, &w, &h); // calc width of new string
  // tft.setCursor(x - w / 2, y);
  // tft.print(buf.c_str());

  // xSemaphoreGive(spiBus.mutex);
  // // CRITICAL SECTION SPI: end
}

int DriverDisplay::_getColorForInfoType(INFO_TYPE type) {
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

// commented out code is preparation for font usage
void DriverDisplay::write_driver_info() {
  string msg = carState.DriverInfo.get();
  INFO_TYPE type = carState.DriverInfoType.get();
  if (msg != carState.DriverInfo.get_last()) {
    int len = msg.length();
    int textSize = infoTextSize;
    if (len > 2 * 17)
      textSize = 2;
    if (len > 4 * 26)
      textSize = 1;
    if (len > 7 * 53)
      msg = msg.substr(0, 7 * 53 - 3) + "...";
    xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
    tft.fillRect(infoFrameX, infoFrameY, infoFrameSizeX, infoFrameSizeY, bgColor);
    // tft.setFont(&FreeSans18pt7b);
    tft.setTextSize(textSize);
    tft.setTextWrap(true);
    tft.setTextColor(_getColorForInfoType(type));
    tft.setCursor(infoFrameX, infoFrameY);
    tft.print(msg.c_str());
    xSemaphoreGive(spiBus.mutex);
    carState.DriverInfo.overtake_recent_to_last();
  }
}

void DriverDisplay::driver_display_demo_screen() {
  // printf("  Draw demo screen:\n");
  // printf("   - driver info\n");
  // write_driver_info("123456789_123456789_123456");
  // printf("   - hazzard warn\n");
  // indicator_set_and_blink(INDICATOR::WARN, true);
  // printf("   - speed\n");
  // write_speed(888);
  // printf("   - acceleration\n");
  // write_acceleration(888);
  // printf("   - increase arrow\n");
  // arrow_increase(true);
  // printf("   - decrease arrow\n");
  // arrow_decrease(true);
  // // printf("   - light1 on\n");
  // // LIGHT oldLight = carState.Light.get();
  // // carState.Light.set(LIGHT::L1);
  // // show_light();
  // // sleep(250);
  // // carState.Light.set(LIGHT::L2);
  // // show_light();
  // // sleep(250);
  // // carState.Light.set(oldLight);
  // // show_light();
  // // printf("   - constant mode speed\n");
  // // constant_drive_mode_set(CONSTANT_MODE::SPEED);
  // // constant_drive_mode_show();
  // printf("   - drive direction forwards\n");
  // write_drive_direction(DRIVE_DIRECTION::FORWARD);
  // printf("   - battery\n");
  // write_bat(-8888.8);
  // printf("   - photovoltaic\n");
  // write_pv(-8888.8);
  // printf("   - motor\n");
  // write_motor(-8888.8);
  // printf("   - constant mode power\n");
  // // constant_drive_mode_set(CONSTANT_MODE::POWER);
  // constant_drive_mode_show();
  // printf("   - drive direction backwards\n");
  // write_drive_direction(DRIVE_DIRECTION::BACKWARD);
  // printf("  End of demo screen.\n");
}

DISPLAY_STATUS DriverDisplay::task(DISPLAY_STATUS status, int lifeSignCounter) {
  // printf("DriverDisplay, status: %s\n", DISPLAY_STATUS_str[(int)status]);
  switch (status) {
  // initializing states:
  case DISPLAY_STATUS::SETUPDRIVER:
    re_init();
    display_setup(status);
    status = DISPLAY_STATUS::BACKGROUNDDRIVER;
    break;
  case DISPLAY_STATUS::DEMOSCREEN:
    draw_display_background();
    driver_display_demo_screen();
    status = DISPLAY_STATUS::SETUPDRIVER;
    break;
  case DISPLAY_STATUS::BACKGROUNDDRIVER:
    clear_screen(bgColor);
    draw_display_background();

    BatteryVoltage.showLabel(tft);
    BatteryOn.showLabel(tft);
    PhotoVoltaicCurrent.showLabel(tft);
    PhotoVoltaicOn.showLabel(tft);
    MotorCurrent.showLabel(tft);
    MotorOn.showLabel(tft);

    status = DISPLAY_STATUS::DRIVER;
    break;
  // working state:
  case DISPLAY_STATUS::DRIVER:
    if (lifeSignCounter > 10) {
      write_driver_info();
      write_speed();
      write_acceleration();

      write_drive_direction();
      show_light();
      constant_drive_mode_show();

      BatteryOn.Value = carState.BatteryOn.get();
      BatteryVoltage.Value = carState.BatteryVoltage.get();
      PhotoVoltaicOn.Value = carState.PhotoVoltaicOn.get();
      PhotoVoltaicCurrent.Value = carState.PhotoVoltaicCurrent.get();
      MotorOn.Value = carState.MotorOn.get();
      MotorCurrent.Value = carState.MotorCurrent.get();

      BatteryVoltage.showValue(tft);
      BatteryOn.showValue(tft);
      PhotoVoltaicCurrent.showValue(tft);
      PhotoVoltaicOn.showValue(tft);
      MotorCurrent.showValue(tft);
      MotorOn.showValue(tft);
    }
    break;
  default:
    // ignore others
    break;
  }
  return status;
}

// } //namespace Display
