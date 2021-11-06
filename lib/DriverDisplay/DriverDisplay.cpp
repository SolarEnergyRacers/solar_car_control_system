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
  try {
    xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
    height = tft.height();
    width = tft.width();
    tft.setRotation(1);
    tft.setTextSize(1);
    tft.setTextColor(ILI9341_BLUE);
    tft.setScrollMargins(10, width - 20);
    infoFrameSizeX = width;
    speedFrameX = (width - speedFrameSizeX) / 2;
    xSemaphoreGive(spiBus.mutex);
  } catch (__exception ex) {
    xSemaphoreGive(spiBus.mutex);
    printf("[x] DriverDisplay: Unable to initialize screen ILI9341.\n");
    throw ex;
  }
  printf("[v] DriverDisplay inited: screen ILI9341 with %d x %d.\n", height, width);
  return DISPLAY_STATUS::CONSOLE;
}

// void DriverDisplay::print(string msg) {
//   if (!_is_ready())
//     return;
//   if (get_DisplayStatus() == DISPLAY_STATUS::CONSOLE) {
//     xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
//     tft.setTextSize(1);
//     tft.print(msg.c_str());
//     xSemaphoreGive(spiBus.mutex);
//   } else {
//     carState.DriverInfo.set(msg);
//     carState.DriverInfoType.set(INFO_TYPE::INFO);
//     write_driver_info();
//   }
// }

void DriverDisplay::speedCheck(int speed) {
  if (!_is_ready())
    return;
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
  accFrameSizeX = speedFrameX - 3;
  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
  tft.drawRoundRect(accFrameX, accFrameY, accFrameSizeX, accFrameSizeY, 4, color);
  xSemaphoreGive(spiBus.mutex);
}

void DriverDisplay::draw_display_background() {
  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
  infoFrameSizeX = tft.width();
  speedFrameX = (tft.width() - speedFrameSizeX) / 2;
  xSemaphoreGive(spiBus.mutex);

  draw_display_border(ILI9341_GREEN);
  draw_speed_border(ILI9341_YELLOW);
  draw_acceleration_border(ILI9341_YELLOW);
}

void DriverDisplay::_arrow_increase(int color) {
  if (!_is_ready())
    return;
  int x = speedFrameX;
  int y = speedFrameY - 3;

  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
  tft.fillTriangle(x, y, x + speedFrameSizeX, y, x + speedFrameSizeX / 2, y - 10, color);
  xSemaphoreGive(spiBus.mutex);
}

void DriverDisplay::_arrow_decrease(int color) {
  if (!_is_ready())
    return;
  int x = speedFrameX;
  int y = speedFrameY + speedFrameSizeY + 3;

  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
  tft.fillTriangle(x, y, x + speedFrameSizeX, y, x + speedFrameSizeX / 2, y + 10, color);
  xSemaphoreGive(spiBus.mutex);
}

// show the slower arrow (red under the speed display)
void DriverDisplay::arrow_decrease(bool on) {
  if (!_is_ready())
    return;
  int color = bgColor;
  if (on) {
    arrow_increase(false);
    color = ILI9341_RED;
  }
  _arrow_decrease(color);
}

// show the faster arrow (green above the speed display)
void DriverDisplay::arrow_increase(bool on) {
  if (!_is_ready())
    return;
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
  if (!_is_ready())
    return;
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
  if (!_is_ready())
    return;
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
  if (!_is_ready())
    return;
  int x = indicatorLeftX;
  int y = indicatorY;

  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
  tft.fillTriangle(x, y, x + indicatorWidth, y - indicatorHeight, x + indicatorWidth, y + indicatorHeight, color);
  xSemaphoreGive(spiBus.mutex);
}

void DriverDisplay::_turn_Right(int color) {
  if (!_is_ready())
    return;
  int x = indicatorRightX;
  int y = indicatorY;

  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
  tft.fillTriangle(x, y, x - indicatorWidth, y - indicatorHeight, x - indicatorWidth, y + indicatorHeight, color);
  xSemaphoreGive(spiBus.mutex);
}

void DriverDisplay::indicator_set_and_blink(INDICATOR direction) { indicator_set_and_blink(direction, true); }

void DriverDisplay::indicator_set_and_blink(INDICATOR direction, bool blinkOn) {
  if (!_is_ready())
    return;
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
  if (!_is_ready())
    return;
  int width = getPixelWidthOfTexts(lightTextSize, LIGHT1_STRING, LIGHT2_STRING) + 4;
  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
  tft.fillRoundRect(lightX - 2, lightY - 2, width, 18, 3, ILI9341_BLACK);
  xSemaphoreGive(spiBus.mutex);
}

void DriverDisplay::show_light() {
  if (!_is_ready())
    return;
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
  if (!_is_ready())
    return;
  int value = carState.Speed.get();
  if (abs(speedLast - value) < 1)
    return;
  if (value < 0 || value > 999)
    value = 999;
  debug_printf("Speed: %d\n", value);
  speedLast = write_nat_999(speedFrameX + 9, speedFrameY + 10, speedLast, value, speedTextSize, ILI9341_WHITE);
}

// acceleration value: -99...+99
void DriverDisplay::write_acceleration() {
  if (!_is_ready())
    return;
  int value = carState.AccelerationDisplay.get();
  if (value > -99 && value < 99) {
    accelerationLast = write_ganz_99(accFrameX + 4, accFrameY + 4, accelerationLast, value, accTextSize, ILI9341_GREENYELLOW);
  }
}

// commented out code is preparation for font usage
void DriverDisplay::write_driver_info() {
  if (!_is_ready())
    return;
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
    tft.setTextColor(getColorForInfoType(type));
    tft.setCursor(infoFrameX, infoFrameY);
    tft.print(msg.c_str());
    xSemaphoreGive(spiBus.mutex);
    carState.DriverInfo.overtake_recent_to_last();
  }
}

void DriverDisplay::driver_display_demo_screen() {
  if (!_is_ready())
    return;
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
  // case DISPLAY_STATUS::DEMOSCREEN:
  //   draw_display_background();
  //   driver_display_demo_screen();
  //   status = DISPLAY_STATUS::SETUPDRIVER;
  //   break;
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
    break;
  default:
    // ignore others
    break;
  }
  return status;
}

// } //namespace Display
