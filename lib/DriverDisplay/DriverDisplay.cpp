/***
 *
 * Driver's Display
 *
 * The driver's display is passive with the exception of the indicator function.
 * This means that all sensors send their values to the display.
 *
 ***/

// standard libraries
#include <iostream>
#include <stdio.h>
#include <string>

#include <LocalFunctionsAndDevices.h>

#include <abstract_task.h>
#include <definitions.h>

// #include <ADC.h>
#include <CarState.h>
#include <Display.h>
#include <DriverDisplay.h>
#include <Helper.h>

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
extern CarState carState;
extern Adafruit_ILI9341 tft;

DISPLAY_STATUS DriverDisplay::display_setup() {
  cout << "[v] '" << getName() << "' inited: screen E ILI9341 with " << height << " x " << width << "." << endl;
  return DISPLAY_STATUS::DRIVER_BACKGROUND;
}

int DriverDisplay::getColorForInfoType(INFO_TYPE type) {
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

// write color of the border of the main display
void DriverDisplay::draw_display_border(int color) {
  xSemaphoreTakeT(spiBus.mutex);
  tft.drawRoundRect(0, mainFrameX, tft.width(), tft.height() - mainFrameX, 8, color);
  xSemaphoreGive(spiBus.mutex);
}

// write color of the border of the speed display
void DriverDisplay::draw_speed_border(int color) {
  xSemaphoreTakeT(spiBus.mutex);
  tft.drawRoundRect(speedFrameX, speedFrameY, speedFrameSizeX, speedFrameSizeY, 4, color);
  xSemaphoreGive(spiBus.mutex);
}

// write color of the border of the acceleration display
void DriverDisplay::draw_acceleration_border(int color) {
  accFrameSizeX = speedFrameX - 3;
  xSemaphoreTakeT(spiBus.mutex);
  tft.drawRoundRect(accFrameX, accFrameY, accFrameSizeX, accFrameSizeY, 4, color);
  xSemaphoreGive(spiBus.mutex);
}

void DriverDisplay::draw_display_background() {
  xSemaphoreTakeT(spiBus.mutex);
  infoFrameSizeX = tft.width();
  speedFrameX = (tft.width() - speedFrameSizeX) / 2;
  xSemaphoreGive(spiBus.mutex);

  draw_display_border(ILI9341_GREEN);
  draw_speed_border(ILI9341_YELLOW);
  draw_acceleration_border(ILI9341_YELLOW);
}

void DriverDisplay::_arrow_increase(int color) {
  int x = speedFrameX;
  int y = speedFrameY - 3;

  xSemaphoreTakeT(spiBus.mutex);
  tft.fillTriangle(x, y, x + speedFrameSizeX, y, x + speedFrameSizeX / 2, y - 10, color);
  xSemaphoreGive(spiBus.mutex);
}

void DriverDisplay::_arrow_decrease(int color) {
  int x = speedFrameX;
  int y = speedFrameY + speedFrameSizeY + 3;

  xSemaphoreTakeT(spiBus.mutex);
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
  CONSTANT_MODE mode = ConstantMode.get_recent_overtake_last();
  bool isOn = ConstantModeOn.get_recent_overtake_last();
  int width = getPixelWidthOfTexts(constantModeTextSize, SPEED_STRING, POWER_STRING) + 4;
  if (mode == CONSTANT_MODE::NONE || !isOn) {
    xSemaphoreTakeT(spiBus.mutex);
    tft.fillRoundRect(constantModeX - 2, constantModeY - 2, width, 18, 3, ILI9341_BLACK);
    xSemaphoreGive(spiBus.mutex);
    return;
  }

  xSemaphoreTakeT(spiBus.mutex);
  tft.fillRoundRect(constantModeX - 2, constantModeY - 2, width, 18, 3, ILI9341_YELLOW);
  tft.setCursor(constantModeX, constantModeY);
  tft.setTextSize(constantModeTextSize);
  tft.setTextColor(ILI9341_BLACK);
  if (mode == CONSTANT_MODE::POWER) {
    tft.print(POWER_STRING);
  } else {
    tft.print(SPEED_STRING);
  }
  // tft.setTextSize(1);
  xSemaphoreGive(spiBus.mutex);
}

#define LIGHT1_STRING "Light"
#define LIGHT2_STRING "LIGHT"
void DriverDisplay::_hide_light() {
  int width = getPixelWidthOfTexts(lightTextSize, LIGHT1_STRING, LIGHT2_STRING) + 4;
  xSemaphoreTakeT(spiBus.mutex);
  tft.fillRoundRect(lightX - 2, lightY - 2, width, 18, 3, ILI9341_BLACK);
  xSemaphoreGive(spiBus.mutex);
}

void DriverDisplay::show_light() {
  LIGHT light = Light.get_recent_overtake_last();
  if (light == LIGHT::OFF) {
    _hide_light();
    return;
  }
  int width = getPixelWidthOfTexts(lightTextSize, LIGHT1_STRING, LIGHT2_STRING) + 4;
  xSemaphoreTakeT(spiBus.mutex);
  tft.setCursor(lightX, lightY);
  tft.setTextSize(lightTextSize);
  tft.fillRoundRect(lightX - 2, lightY - 2, width, 18, 3, ILI9341_BLACK);
  if (light == LIGHT::L1) {
    tft.setTextColor(ILI9341_YELLOW);
    tft.print(LIGHT1_STRING);
  } else {
    tft.setTextColor(ILI9341_BLUE);
    tft.print(LIGHT2_STRING);
  }
  xSemaphoreGive(spiBus.mutex);
}

#define FORWARD_STRING ""
#define BACKWARD_STRING "Backward"
void DriverDisplay::write_drive_direction() {
  DRIVE_DIRECTION direction = DriveDirection.get_recent_overtake_last();
  int width = getPixelWidthOfTexts(driveDirectionTextSize, FORWARD_STRING, BACKWARD_STRING) + 4;

  xSemaphoreTakeT(spiBus.mutex);
  tft.fillRoundRect(driveDirectionX - 2, driveDirectionY - 2, width, 18, 3, ILI9341_BLACK);
  tft.setTextSize(driveDirectionTextSize);
  tft.setCursor(driveDirectionX, driveDirectionY);
  if (direction == DRIVE_DIRECTION::FORWARD) {
    tft.setTextColor(ILI9341_YELLOW);
    tft.print(FORWARD_STRING);
  } else {
    tft.setTextColor(ILI9341_RED);
    tft.print(BACKWARD_STRING);
  }
  xSemaphoreGive(spiBus.mutex);
}

void DriverDisplay::_turn_Left(int color) {
  int x = indicatorLeftX;
  int y = indicatorY;

  xSemaphoreTakeT(spiBus.mutex);
  tft.fillTriangle(x, y, x + indicatorWidth, y - indicatorHeight, x + indicatorWidth, y + indicatorHeight, color);
  xSemaphoreGive(spiBus.mutex);
}

void DriverDisplay::_turn_Right(int color) {
  int x = indicatorRightX;
  int y = indicatorY;

  xSemaphoreTakeT(spiBus.mutex);
  tft.fillTriangle(x, y, x - indicatorWidth, y - indicatorHeight, x - indicatorWidth, y + indicatorHeight, color);
  xSemaphoreGive(spiBus.mutex);
}

void DriverDisplay::show_indicator() {
  INDICATOR indicator = Indicator.get_recent_overtake_last();
  switch (indicator) {
  case INDICATOR::OFF:
    _turn_Left(bgColor);
    _turn_Right(bgColor);
    break;

  case INDICATOR::LEFT:
    _turn_Left(ILI9341_YELLOW);
    _turn_Right(bgColor);
    break;

  case INDICATOR::RIGHT:
    _turn_Left(bgColor);
    _turn_Right(ILI9341_YELLOW);
    break;

  case INDICATOR::WARN:
    _turn_Left(ILI9341_RED);
    _turn_Right(ILI9341_RED);
    break;

  default:
    break;
  }
}

// Write the speed in the centre frame: 0...999
void DriverDisplay::write_speed() {
  int value = Speed.get_recent_overtake_last();
  if (value >= 0 || value <= 999) {
    if (justInited)
      speedLast = -1;
    speedLast = write_nat_999(speedFrameX + 9, speedFrameY + 10, speedLast, value, speedTextSize, ILI9341_WHITE);
  }
}

// acceleration value: -99...+99
void DriverDisplay::write_acceleration() {
  int value = Acceleration.get_recent_overtake_last();
  if (value >= -99 && value <= 99) {
    accelerationLast = write_ganz_99(accFrameX + 4, accFrameY + 4, accelerationLast, value, accTextSize, justInited, ILI9341_GREENYELLOW);
  }
}

// commented out code is preparation for font usage
void DriverDisplay::write_driver_info() {
  if (DriverInfo.Value != DriverInfo.ValueLast || justInited) {
    string msg = DriverInfo.get_recent_overtake_last();
    INFO_TYPE type = DriverInfoType.Value;
    int len = msg.length();
    int textSize = infoTextSize;
    if (len > 2 * 17)
      textSize = 2;
    if (len > 4 * 26)
      textSize = 1;
    if (len > 7 * 53)
      msg = msg.substr(0, 7 * 53 - 3) + "...";
    xSemaphoreTakeT(spiBus.mutex);
    tft.fillRect(infoFrameX, infoFrameY, infoFrameSizeX, infoFrameSizeY, bgColor);
    // tft.setFont(&FreeSans18pt7b);
    tft.setTextSize(textSize);
    tft.setTextWrap(true);
    tft.setTextColor(getColorForInfoType(type));
    tft.setCursor(infoFrameX, infoFrameY);
    tft.print(msg.c_str());
    xSemaphoreGive(spiBus.mutex);
  }
}

DISPLAY_STATUS DriverDisplay::task(int lifeSignCounter) {
  DISPLAY_STATUS status = carState.displayStatus;
  switch (carState.displayStatus) {
  // initializing states:
  case DISPLAY_STATUS::DRIVER_SETUP:
    re_init();
    display_setup();
    justInited = true;
    status = DISPLAY_STATUS::DRIVER_BACKGROUND;
    break;

  case DISPLAY_STATUS::DRIVER_BACKGROUND:
    clear_screen(bgColor);
    draw_display_background();

    BatteryVoltage.showLabel(tft);
    BatteryVoltage.set_epsilon(0.1);
    BatteryOn.showLabel(tft);
    PhotoVoltaicCurrent.showLabel(tft);
    PhotoVoltaicCurrent.set_epsilon(0.1);
    PhotoVoltaicOn.showLabel(tft);
    MotorCurrent.showLabel(tft);
    MotorCurrent.set_epsilon(0.1);
    MotorOn.showLabel(tft);
    set_SleepTime(300);
    status = DISPLAY_STATUS::DRIVER_RUNNING;
    break;

  // working state:
  case DISPLAY_STATUS::DRIVER_RUNNING:
    DriverInfo.Value = carState.DriverInfo;
    if (DriverInfo.Value != DriverInfo.ValueLast || justInited) {
      write_driver_info();
    }
    Speed.Value = carState.Speed;
    if (Speed.is_changed() || justInited) {
      write_speed();
    }
    Acceleration.Value = carState.AccelerationDisplay;
    if (Acceleration.is_changed() || justInited) {
      write_acceleration();
    }
    DriveDirection.Value = carState.DriveDirection;
    if (DriveDirection.Value != DriveDirection.ValueLast || justInited) {
      write_drive_direction();
    }
    Light.Value = carState.Light;
    if (Light.Value != Light.ValueLast || justInited) {
      show_light();
    }
    ConstantMode.Value = carState.ConstantMode;
    ConstantModeOn.Value = carState.ConstantModeOn;
    if (ConstantMode.Value != ConstantMode.ValueLast || ConstantModeOn.Value != ConstantModeOn.ValueLast || justInited) {
      constant_drive_mode_show();
    }

    Indicator.Value = carState.Indicator;
    if (Indicator.Value != Indicator.ValueLast || justInited) {
      show_indicator();
    }

    BatteryVoltage.Value = carState.BatteryVoltage;
    if (BatteryVoltage.is_changed() || justInited) {
      BatteryVoltage.showValue(tft);
    }
    BatteryOn.Value = carState.BatteryOn;
    if (BatteryOn.is_changed() || justInited) {
      BatteryOn.showValue(tft);
    }
    PhotoVoltaicCurrent.Value = carState.PhotoVoltaicCurrent;
    if (PhotoVoltaicCurrent.is_changed() || justInited) {
      PhotoVoltaicCurrent.showValue(tft);
    }
    PhotoVoltaicOn.Value = carState.PhotoVoltaicOn;
    if (PhotoVoltaicOn.is_changed() || justInited) {
      PhotoVoltaicOn.showValue(tft);
    }
    MotorCurrent.Value = carState.MotorCurrent;
    if (MotorCurrent.is_changed() || justInited) {
      MotorCurrent.showValue(tft);
    }
    MotorOn.Value = carState.MotorOn;
    if (MotorOn.is_changed() || justInited) {
      MotorOn.showValue(tft);
    }
    justInited = false;
    break;

  default:
    // ignore others
    break;
  }
  return status;
}
