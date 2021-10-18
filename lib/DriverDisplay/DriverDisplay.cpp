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
extern bool systemOk;
extern CarState carState;
extern DriverDisplay dd;

Adafruit_ILI9341 tft = Adafruit_ILI9341(0, 0, 0, 0, 0, 0);
// namespace DriverDisplay {

//==== display cache =====================
// ... to avoid flickering
int lifeSignCounter = 0;
bool lifeSignState = false;
string infoLast = "";
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

// DriverDisplay *DriverDisplay::_instance = 0;
DriverDisplay::DISPLAY_STATUS DriverDisplay::status = DISPLAY_STATUS::SETUP;

string DriverDisplay ::getName() { return "Display0 (driver display)"; };

void DriverDisplay ::init() {
  abstract_task::init();
  status = DISPLAY_STATUS::DISPLAY_CONSOLE;
  re_init();
}

void DriverDisplay ::re_init(void) { _setup(); }

void DriverDisplay ::_setup() {
  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
  tft = Adafruit_ILI9341(SPI_CS_TFT, SPI_DC, SPI_MOSI, SPI_CLK, SPI_RST, SPI_MISO);
  tft.begin();
  // read for bugs: https://forums.adafruit.com/viewtopic.php?p=815969
  // tft.initSPI(3000000, SPI_MODE3);
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
    // tft.fillScreen(bgColor);
    tft.setRotation(1);
    tft.setTextSize(1);
    tft.setTextColor(ILI9341_WHITE);
    tft.setScrollMargins(10, tft.width() - 20);
    infoFrameSizeX = tft.width();
    speedFrameX = (tft.width() - speedFrameSizeX) / 2;
    tft.printf("Screen ILI9341 inited with %d x %d.\n", tft.height(), tft.width());
  } catch (__exception ex) {
    printf("[x] Display0 (driver display): Unable to initialize screen ILI9341.\n");
    throw ex;
  }
  xSemaphoreGive(spiBus.mutex);
  printf("[v] Display0 (driver display) inited: screen ILI9341 with %d x %d.\n", tft.height(), tft.width());
}

void DriverDisplay::clear_screen(int bgColor){
  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
  tft.fillScreen(bgColor);
  xSemaphoreGive(spiBus.mutex);
}

void DriverDisplay ::exit() {}

// -------------
// FreeRTOS TASK
// -------------
void DriverDisplay ::task(void) {
  // polling loop
  while (1) {
    switch (status) {
    // initializing states:
    case DISPLAY_STATUS::DISPLAY_DEMOSCREEN:
      clear_screen(bgColor);
      draw_display_background();
      driver_display_demo_screen();
      status = DISPLAY_STATUS::DISPLAY_BACKGROUND;
      break;
    case DISPLAY_STATUS::DISPLAY_BACKGROUND:
      draw_display_background();
      status = DISPLAY_STATUS::DRIVER;
      break;
    // working states:
    case DISPLAY_STATUS::DISPLAY_CONSOLE:
      // do nothing special
      break;
    case DISPLAY_STATUS::ENGINEER:
      break;
    case DISPLAY_STATUS::DRIVER:
    default: // driver / engineer screen
      if (lifeSignCounter > 10) {
        lifeSign();
        lifeSignCounter = 0;
        int accDisplayValue = adc.read_adc_acc_dec();
        write_acceleration(accDisplayValue);
        // speedCheck(speedLast);
      }
      lifeSignCounter++;
      break;
    }
    // sleep for sleep_polling_ms
    this->sleep(20);
  }
}

void DriverDisplay::print(string msg) {
  if (status == DISPLAY_STATUS::DISPLAY_CONSOLE) {
    tft.print(msg.c_str());
  } else {
    write_driver_info(msg, INFO_TYPE::STATUS);
  }
}

//------------------------------------------------------------------------
void DriverDisplay::setupScrollArea(uint16_t TFA, uint16_t BFA) {
  tft.writeCommand(ILI9341_VSCRDEF); // Vertical scroll definition
  tft.write(TFA >> 8);
  tft.write(TFA);
  tft.write((320 - TFA - BFA) >> 8);
  tft.write(320 - TFA - BFA);
  tft.write(BFA >> 8);
  tft.write(BFA);
}

int DriverDisplay::scroll(int lines) {
  int TEXT_HEIGHT = 8;    // Height of text to be printed and scrolled
  int BOT_FIXED_AREA = 0; // Number of lines in bottom fixed area (lines counted from bottom of screen)
  int TOP_FIXED_AREA = 0;
  uint16_t yStart = TOP_FIXED_AREA;
  uint16_t yArea = 320 - TOP_FIXED_AREA - BOT_FIXED_AREA;
  uint16_t yDraw = 320 - BOT_FIXED_AREA - TEXT_HEIGHT;
  int yTemp = yStart;
  for (int i = 0; i < lines; i++) {
    yStart++;
    if (yStart == 320 - BOT_FIXED_AREA)
      yStart = TOP_FIXED_AREA;
    scrollAddress(yStart);
  }
  return yTemp;
}

void DriverDisplay::scrollAddress(uint16_t VSP) {
  tft.writeCommand(ILI9341_VSCRSADD); // Vertical scrolling start address
  tft.write(VSP >> 8);
  tft.write(VSP);
}

//________________________________________________________________________

void DriverDisplay::speedCheck(int speed) {
  if (speed < 50) {
    dd.arrow_increase(true);
  } else {
    dd.arrow_increase(false);
  }
  if (speed > 80) {
    dd.arrow_decrease(true);
  } else {
    dd.arrow_decrease(false);
  }
}

void exit(void) {}

// writes DriverDisplay ::float value  in the range from -9999.9 to 9999.9
float DriverDisplay ::_write_float(int x, int y, float valueLast, float value, int textSize, int color) {

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
int DriverDisplay ::_write_ganz_99(int x, int y, int valueLast, int value, int textSize, int color) {
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
int DriverDisplay ::_write_nat_999(int x, int y, int valueLast, int value, int textSize, int color) {
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
void DriverDisplay ::draw_display_border(int color) {
  // CRITICAL SECTION SPI: start
  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);

  tft.drawRoundRect(0, mainFrameX, tft.width(), tft.height() - mainFrameX, 8, color);

  xSemaphoreGive(spiBus.mutex);
  // CRITICAL SECTION SPI: end
}

// write color of the border of the speed display
void DriverDisplay ::draw_speed_border(int color) {
  // CRITICAL SECTION SPI: start
  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);

  tft.drawRoundRect(speedFrameX, speedFrameY, speedFrameSizeX, speedFrameSizeY, 4, color);

  xSemaphoreGive(spiBus.mutex);
  // CRITICAL SECTION SPI: end
}

// write color of the border of the speed display
void DriverDisplay ::draw_acceleration_border(int color) {
  // CRITICAL SECTION SPI: start
  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);

  accFrameSizeX = speedFrameX - 3;
  tft.drawRoundRect(accFrameX, accFrameY, accFrameSizeX, accFrameSizeY, 4, color);

  xSemaphoreGive(spiBus.mutex);
  // CRITICAL SECTION SPI: end
}

void DriverDisplay ::lifeSign() {
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

void DriverDisplay ::draw_display_background() {
  clear_screen(bgColor);
  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);

  //tft.fillScreen(bgColor);
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

  string infoLast = "";
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

void DriverDisplay ::_arrow_increase(int color) {
  int x = speedFrameX;
  int y = speedFrameY - 3;

  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
  tft.fillTriangle(x, y, x + speedFrameSizeX, y, x + speedFrameSizeX / 2, y - 10, color);
  xSemaphoreGive(spiBus.mutex);
}

void DriverDisplay ::_arrow_decrease(int color) {
  int x = speedFrameX;
  int y = speedFrameY + speedFrameSizeY + 3;

  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
  tft.fillTriangle(x, y, x + speedFrameSizeX, y, x + speedFrameSizeX / 2, y + 10, color);
  xSemaphoreGive(spiBus.mutex);
}

// show the slower arrow (red under the speed display)
void DriverDisplay ::arrow_decrease(bool on) {
  int color = bgColor;
  if (on) {
    arrow_increase(false);
    color = ILI9341_RED;
  }
  _arrow_decrease(color);
}

// show the faster arrow (green above the speed display)
void DriverDisplay ::arrow_increase(bool on) {
  int color = bgColor;
  if (on) {
    arrow_decrease(false);
    color = ILI9341_YELLOW;
  }
  _arrow_increase(color);
}

void DriverDisplay ::constant_drive_mode_set(CONSTANT_MODE mode) {
  constant_drive_mode = mode;
  if (constant_drive_set == true) {
    constant_drive_mode_show();
  }
}

void DriverDisplay ::constant_drive_on() {
  constant_drive_set = true;
  constant_drive_mode_show();
}

void DriverDisplay ::constant_drive_off() {
  constant_drive_set = false;
  constant_drive_mode_hide();
}

void DriverDisplay ::constant_drive_mode_hide() {
  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
  tft.setTextSize(constantModeTextSize);
  tft.setTextColor(ILI9341_BLACK);
  tft.setCursor(constantModeX, constantModeY);
  tft.print("power");
  tft.setCursor(constantModeX, constantModeY);
  tft.print("speed");
  xSemaphoreGive(spiBus.mutex);
}

void DriverDisplay ::constant_drive_mode_show() {
  constant_drive_mode_hide();
  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
  tft.setTextColor(ILI9341_YELLOW);
  tft.setCursor(constantModeX, constantModeY);
  if (carState.ConstantMode.get() == CONSTANT_MODE::POWER) {
    tft.print("power");
  } else {
    tft.print("speed");
  }
  xSemaphoreGive(spiBus.mutex);
}

void DriverDisplay ::write_drive_direction(DRIVE_DIRECTION direction) {
  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);

  tft.setTextSize(driveDirectionTextSize);
  tft.setTextColor(ILI9341_BLACK);
  tft.setCursor(driveDirectionX, driveDirectionY);
  tft.print("forward");
  tft.setCursor(driveDirectionX, driveDirectionY);
  tft.print("backward");

  tft.setCursor(driveDirectionX, driveDirectionY);
  if (carState.DriveDirection.get() == DRIVE_DIRECTION::FORWARD) {
    tft.setTextColor(ILI9341_YELLOW);
    tft.print("forward");
  } else {
    tft.setTextColor(ILI9341_RED);
    tft.print("backward");
  }
  xSemaphoreGive(spiBus.mutex);
}

void DriverDisplay ::_turn_Left(int color) {
  int x = indicatorLeftX;
  int y = indicatorY;

  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
  tft.fillTriangle(x, y, x + indicatorWidth, y - indicatorHeight, x + indicatorWidth, y + indicatorHeight, color);
  xSemaphoreGive(spiBus.mutex);
}

void DriverDisplay ::_turn_Right(int color) {
  int x = indicatorRightX;
  int y = indicatorY;

  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
  tft.fillTriangle(x, y, x - indicatorWidth, y - indicatorHeight, x - indicatorWidth, y + indicatorHeight, color);
  xSemaphoreGive(spiBus.mutex);
}

void DriverDisplay ::indicator_set_and_blink(INDICATOR direction) { indicator_set_and_blink(direction, true); }

void DriverDisplay ::indicator_set_and_blink(INDICATOR direction, bool blinkOn) {
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

void DriverDisplay ::_light1(bool lightOn) {
  light1On = !lightOn;
  light1OnOff();
}

void DriverDisplay ::_light2(bool lightOn) {
  light2On = !lightOn;
  light2OnOff();
}

void DriverDisplay ::light1OnOff() {
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

void DriverDisplay ::light2OnOff() {
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
void DriverDisplay ::write_speed() {
  int value = carState.Speed.get();
  write_speed(value);
}

void DriverDisplay ::write_speed(int value) {
  speedLast = _write_nat_999(speedFrameX + 9, speedFrameY + 10, speedLast, value, speedTextSize, ILI9341_WHITE);

  // tft.setFont(&FreeMonoBold24pt7b);
  // speedLast = _write_nat_999(speedFrameX + 20, speedFrameY + 25, speedLast,
  // value, 2, ILI9341_WHITE);
  // tft.setFont();
}

// acceleration value: 0-255
void DriverDisplay ::write_acceleration(int value) {
  if (value < -99 || value > 99) {
    value = 999;
  }
  accelerationLast = _write_ganz_99(accFrameX + 4, accFrameY + 4, accelerationLast, value, accTextSize, ILI9341_GREENYELLOW);
}

void DriverDisplay ::write_bat(float value) {
  int labelOffset = labelLen * batTextSize * 6;
  batLast = _write_float(batFrameX + labelOffset, batFrameY, batLast, value, batTextSize, ILI9341_BLUE);
}

void DriverDisplay ::write_pv(float value) {
  int labelOffset = labelLen * pvTextSize * 6;
  pvLast = _write_float(pvFrameX + labelOffset, pvFrameY, pvLast, value, pvTextSize, ILI9341_YELLOW);
}

void DriverDisplay ::write_motor(float value) {
  int labelOffset = labelLen * motorTextSize * 6;
  motorLast = _write_float(motorFrameX + labelOffset, motorFrameY, motorLast, value, motorTextSize, ILI9341_YELLOW);
}

void DriverDisplay ::_drawCentreString(const string &buf, int x, int y) {
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

// INFO = ILI9341_WHITE, STATUS = ILI9341_GREEN, WARN = ILI9341_PURPLE, ERROR
// = ILI9341_RED
int DriverDisplay ::_getColorForInfoType(INFO_TYPE type) {
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

void DriverDisplay ::write_driver_info(string msg, INFO_TYPE type) {
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
    tft.print(infoLast.c_str());

    tft.setTextColor(_getColorForInfoType(type));
    tft.setCursor(infoFrameX, infoFrameY);
    tft.print(msg.c_str());
    // TODO: _drawCentreString(msg.c_str(), 0, 0);
    infoLast = msg;
  }
  // tft.setFont();

  xSemaphoreGive(spiBus.mutex);
  // CRITICAL SECTION SPI: end
}

void DriverDisplay ::driver_display_demo_screen() {
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

// } //namespace DriverDisplay
