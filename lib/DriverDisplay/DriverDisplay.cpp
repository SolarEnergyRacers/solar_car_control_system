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
#include <CarState.h>
#include <Display.h>
#include <DriverDisplay.h>
#include <EngineerDisplay.h>
#include <Helper.h>
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
extern EngineerDisplay engineerDisplay;

DISPLAY_STATUS DriverDisplay::display_setup(DISPLAY_STATUS status) {
  // printf("[?] Setup 'DriverDisplay'...\n");
  // bgColor = ILI9341_BLACK;
  // int height = 0;
  // int width = 0;
  // try {
  //   xSemaphoreTakeT(spiBus.mutex);
  //   height = tft.height();
  //   width = tft.width();
  //   tft.setRotation(1);
  //   tft.setTextSize(1);
  //   tft.setTextColor(ILI9341_BLUE);
  //   tft.setScrollMargins(10, width - 20);
  //   infoFrameSizeX = width;
  //   speedFrameX = (width - speedFrameSizeX) / 2;
  //   xSemaphoreGive(spiBus.mutex);
  // } catch (__exception ex) {
  //   xSemaphoreGive(spiBus.mutex);
  //   printf("[x] DriverDisplay: Unable to initialize screen ILI9341.\n");
  //   throw ex;
  // }
  printf("[v] %s inited: screen ILI9341 with %d x %d.\n", getName().c_str(), height, width);
  return DISPLAY_STATUS::CONSOLE;
}

// void DriverDisplay::print(string msg) {
//   if (get_DisplayStatus() == DISPLAY_STATUS::CONSOLE) {
//     xSemaphoreTakeT(spiBus.mutex);
//     tft.setTextSize(1);
//     tft.print(msg.c_str());
//     xSemaphoreGive(spiBus.mutex);
//   } else {
//     carState.DriverInfo.set(msg);
//     carState.DriverInfoType.set(INFO_TYPE::INFO);
//     write_driver_info();
//   }
// }

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
  CONSTANT_MODE mode = carState.ConstantMode.get_recent_overtake_last();
  bool isOn = carState.ConstantModeOn.get_recent_overtake_last();
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
  LIGHT light = carState.Light.get_recent_overtake_last();
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
  // tft.setTextSize(1);
  xSemaphoreGive(spiBus.mutex);
}

#define FORWARD_STRING ""
#define BACKWARD_STRING "Backward"
void DriverDisplay::write_drive_direction() {
  DRIVE_DIRECTION direction = carState.DriveDirection.get_recent_overtake_last();
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
  INDICATOR indicator = carState.Indicator.get_recent_overtake_last();
  if (indicator != INDICATOR::OFF || blinkOn) {
    _turn_Left(bgColor);
    _turn_Right(bgColor);
  }
  if (blinkOn) {
    switch (indicator) {
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
  blinkOn = !blinkOn;
}

// Write the speed in the centre frame: 0...999
void DriverDisplay::write_speed() {
  int value = carState.Speed.get_recent_overtake_last();
  if (value >= 0 || value <= 999) {
    if (justInited)
      speedLast = -1;
    speedLast = write_nat_999(speedFrameX + 9, speedFrameY + 10, speedLast, value, speedTextSize, ILI9341_WHITE);
  }
}

// acceleration value: -99...+99
void DriverDisplay::write_acceleration() {
  int value = carState.AccelerationDisplay.get_recent_overtake_last();
  if (value >= -99 && value <= 99) {
    if (justInited)
      accelerationLast = -1;
    accelerationLast = write_ganz_99(accFrameX + 4, accFrameY + 4, accelerationLast, value, accTextSize, ILI9341_GREENYELLOW);
  }
}

// commented out code is preparation for font usage
void DriverDisplay::write_driver_info() {
  string msg = carState.DriverInfo.get_recent_overtake_last();
  INFO_TYPE type = carState.DriverInfoType.get();
  if (msg != carState.DriverInfo.get_last() || justInited) {
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
  switch (status) {
  // initializing states:
  case DISPLAY_STATUS::SETUPDRIVER:
    re_init();
    display_setup(status);
    justInited = true;
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
    set_SleepTime(300);
    status = DISPLAY_STATUS::DRIVER;
    break;

  // working state:
  case DISPLAY_STATUS::DRIVER:
    if (carState.DriverInfo.is_changed() || justInited) {
      write_driver_info();
    }
    // if (carState.Speed.is_changed()) {
    if (carState.Speed.is_changed() || justInited) {
      write_speed();
    }
    if (carState.AccelerationDisplay.is_changed() || justInited) {
      write_acceleration();
    }

    if (carState.DriveDirection.is_changed() || justInited) {
      write_drive_direction();
    }
    if (carState.Light.is_changed() || justInited) {
      show_light();
    }
    if (carState.ConstantMode.is_changed() || carState.ConstantModeOn.is_changed() || justInited) {
      constant_drive_mode_show();
    }
    // if (carState.Indicator.is_changed() || justInited) {
    //   printf("Indicator changed");
    //   show_indicator();
    // }
    //show_indicator();

    if (abs(carState.BatteryVoltage.get() - carState.BatteryVoltage.get_last()) > carState.BatteryVoltage.epsilon() || justInited) {
      BatteryVoltage.Value = carState.BatteryVoltage.get_recent_overtake_last();
      BatteryVoltage.showValue(tft);
    }
    if (carState.BatteryOn.is_changed() || justInited) {
      BatteryOn.Value = carState.BatteryOn.get_recent_overtake_last();
      BatteryOn.showValue(tft);
    }
    if (abs(carState.PhotoVoltaicCurrent.get() - carState.PhotoVoltaicCurrent.get_last()) > carState.PhotoVoltaicCurrent.epsilon() ||
        justInited) {
      PhotoVoltaicCurrent.Value = carState.PhotoVoltaicCurrent.get_recent_overtake_last();
      PhotoVoltaicCurrent.showValue(tft);
    }
    if (carState.PhotoVoltaicOn.is_changed() || justInited) {
      PhotoVoltaicOn.Value = carState.PhotoVoltaicOn.get_recent_overtake_last();
      PhotoVoltaicOn.showValue(tft);
    }
    if (abs(carState.MotorCurrent.get() - carState.MotorCurrent.get_last()) > carState.MotorCurrent.epsilon() || justInited) {
      MotorCurrent.Value = carState.MotorCurrent.get_recent_overtake_last();
      MotorCurrent.showValue(tft);
    }
    if (carState.MotorOn.is_changed() || justInited) {
      MotorOn.Value = carState.MotorOn.get_recent_overtake_last();
      MotorOn.showValue(tft);
    }
    justInited = false;
    break;

    // case DISPLAY_STATUS::SETUPENGINEER:
    //   engineerDisplay.create_task(4);
    //   engineerDisplay.set_DisplayStatus(DISPLAY_STATUS::SETUPENGINEER);
    //   break;

  default:
    // ignore others
    break;
  }
  return status;
}
