/*
 * Driver's Display
 *
 * The driver's display is passive.
 * This means that all sensors send their values to the display.
 */

#include <LocalFunctionsAndDevices.h>

#include <abstract_task.h>
#include <definitions.h>

#include <fmt/core.h>
#include <iostream>

// #include <ADC.h>
#include <CarState.h>
#include <Console.h>
#include <Display.h>
#include <Helper.h>
#include <SDCard.h>
#include <SPIBus.h>

#include <ADS1X15.h>
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
extern SDCard sdCard;
extern Console console;

using namespace std;

//==== Display definitions ==== START
// life sign for connection to microprocessor via rtx
int lifeSignX = -1;
int lifeSignY = -1;
int lifeSignRadius = 4;
//==== Display definitions ==== END
//==== Display cache ========== START
// ... to avoid flickering
int lifeSignCounter = 0;
bool lifeSignState = false;
//==== Display cache ========== END

string Display::getName() { return "Display"; };

string Display::init() {
  console << "[  ] Init 'Display'...\n";
  return re_init();
}

string Display::re_init(void) { return _setup(); }

void Display::exit() {
  // todo
}
// ------------

Adafruit_ILI9341 Display::tft = Adafruit_ILI9341(&spiBus.spi, SPI_DC, SPI_CS_TFT, SPI_RST);

string Display::_setup() {
  bool hasError = false;
  console << "     Setup 'ILI9341' for '" << getName() << "' with: SPI_CLK=" << SPI_CLK << ", SPI_MOSI=" << SPI_MOSI
          << ", SPI_MISO=" << SPI_MISO << ", SPI_CS_TFT=" << SPI_CS_TFT << "\n";
  height = 0;
  width = 0;
  try {
    uint8_t rdmode = 0;
    uint8_t rdmadctl = 0;
    uint8_t rdpixfmt = 0;
    uint8_t rdimgfmt = 0;
    uint8_t rdselfdiag = 0;

    xSemaphoreTakeT(spiBus.mutex);
    tft.begin();
    tft.setRotation(1);
    height = tft.height();
    width = tft.width();
    rdmode = tft.readcommand8(ILI9341_RDMODE);
    rdmadctl = tft.readcommand8(ILI9341_RDMADCTL);
    rdpixfmt = tft.readcommand8(ILI9341_RDPIXFMT);
    rdimgfmt = tft.readcommand8(ILI9341_RDIMGFMT);
    rdselfdiag = tft.readcommand8(ILI9341_RDSELFDIAG);

    tft.setCursor(0, 0);
    tft.setTextSize(1);
    tft.fillScreen(bgColor);
    tft.setTextColor(ILI9341_BLUE);
    tft.setScrollMargins(0, height);
    xSemaphoreGive(spiBus.mutex);

    printf("     ILI9341_RDMADCTL:   0x%x\n", rdmadctl);
    printf("     ILI9341_RDPIXFMT:   0x%x\n", rdpixfmt);
    printf("     ILI9341_RDIMGFMT:   0x%x\n", rdimgfmt);
    printf("     ILI9341_RDSELFDIAG: 0x%x\n", rdselfdiag);
    printf("     ILI9341_RDMODE:     0x%x\n", rdmode);
  } catch (exception &ex) {
    console << fmt::format("[--] Display: Unable to initialize screen 'ILI9341': {}\n", ex.what());
    xSemaphoreGive(spiBus.mutex);
    throw ex;
  }
  lifeSignX = width - lifeSignRadius - 4;
  lifeSignY = height - lifeSignRadius - 4;
  return fmt::format("[{}] Display initialized.  Screen 'ILI9341' {}x{}.     Status: {}", hasError ? "--" : "ok", height, width,
                     DISPLAY_STATUS_str[(int)carState.displayStatus]);
}

void Display::clear_screen(int bgColor) {
  xSemaphoreTakeT(spiBus.mutex);
  tft.fillScreen(bgColor);
  xSemaphoreGive(spiBus.mutex);
}

int Display::getPixelWidthOfTexts(int textSize, string t1, string t2) {
  int l1 = t1.length() * textSize * 6;
  int l2 = t2.length() * textSize * 6;
  return max(l1, l2);
}

void Display::print(string msg) {
  if (carState.displayStatus == DISPLAY_STATUS::ENGINEER_CONSOLE) {
    xSemaphoreTakeT(spiBus.mutex);
    tft.setTextSize(1);
    tft.print(msg.c_str());
    xSemaphoreGive(spiBus.mutex);
  }
}

void Display::getCursor(int &x, int &y) {
  xSemaphoreTakeT(spiBus.mutex);
  x = tft.getCursorX();
  y = tft.getCursorY();
  xSemaphoreGive(spiBus.mutex);
}

void Display::setCursor(int x, int y) {
  xSemaphoreTakeT(spiBus.mutex);
  tft.setCursor(x, y);
  xSemaphoreGive(spiBus.mutex);
}

//------------------------------------------------------------------------
void Display::setupScrollArea(uint16_t TFA, uint16_t BFA) {
  xSemaphoreTakeT(spiBus.mutex);
  tft.writeCommand(ILI9341_VSCRDEF); // Vertical scroll definition
  tft.write(TFA >> 8);
  tft.write(TFA);
  tft.write((320 - TFA - BFA) >> 8);
  tft.write(320 - TFA - BFA);
  tft.write(BFA >> 8);
  tft.write(BFA);
  xSemaphoreGive(spiBus.mutex);
}

int Display::scroll(int lines) {
  // int TEXT_HEIGHT = 8;    // Height of text to be printed and scrolled
  int BOT_FIXED_AREA = 0; // Number of lines in bottom fixed area (lines counted from bottom of screen)
  int TOP_FIXED_AREA = 0;
  uint16_t yStart = TOP_FIXED_AREA;
  // uint16_t yArea = 320 - TOP_FIXED_AREA - BOT_FIXED_AREA;
  // uint16_t yDraw = 320 - BOT_FIXED_AREA - TEXT_HEIGHT;
  int yTemp = yStart;
  for (int i = 0; i < lines; i++) {
    yStart++;
    if (yStart == 320 - BOT_FIXED_AREA)
      yStart = TOP_FIXED_AREA;
    scrollAddress(yStart);
  }
  return yTemp;
}

void Display::scrollAddress(uint16_t VSP) {
  xSemaphoreTakeT(spiBus.mutex);
  tft.writeCommand(ILI9341_VSCRSADD); // Vertical scrolling start address
  tft.write(VSP >> 8);
  tft.write(VSP);
  xSemaphoreGive(spiBus.mutex);
}

//________________________________________________________________________

void exit(void) {}

// writes Display::float value  in the range from -9999.9 to 9999.9
float Display::write_float(int x, int y, float valueLast, float value, int textSize, int color) {
  if (value < -9999.9 || value > 9999.9) {
    console << "ERROR: call _write_float with a value outside the range: '" << value << "'\n";
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

  xSemaphoreTakeT(spiBus.mutex);
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
  tft.setTextSize(1);
  xSemaphoreGive(spiBus.mutex);

  return value;
}

// writes integer value in the range from -99 to +99
int Display::write_ganz_99(int x, int y, int valueLast, int value, int textSize, bool justInited, int color, int bgColor) {
  if (value < -99 || value > 999) {
    console << "ERROR: call write_ganz_99 with a value outside the range: '" << value << "'\n";
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

  xSemaphoreTakeT(spiBus.mutex);
  tft.setTextSize(textSize);
  tft.setTextColor(color);
  tft.setCursor(x, y);
  int digitWidth = textSize * 6;
  int digitHeight = textSize * 8;
  // if a change in the digit then replace the old with new value by
  // first deleting the digit area and second write the new value
  if (d1 != d1o || justInited) { //|| d1o == 0) {
    tft.fillRect(x + (digitWidth + 1) * 2, y, digitWidth, digitHeight, bgColor);
    tft.setCursor(x + (digitWidth + 1) * 2, y);
    tft.print(d1);
  }
  if (d2 != d2o || d2o == 0 || justInited) {
    tft.fillRect(x + (digitWidth + 1) * 1, y, digitWidth, digitHeight, bgColor);
    tft.setCursor(x + (digitWidth + 1) * 1, y);
    if (abs(value) > 9) {
      tft.print(d2);
    }
  }
  if (sign != signOld || justInited) {
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
int Display::write_nat_999(int x, int y, int valueLast, int value, int textSize, int color) {
  if (value < 0 || value > 999) {
    console << "ERROR: call _write_nat_999 with a value outside the range: '" << value << "'\n";
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

  xSemaphoreTakeT(spiBus.mutex);
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

#if LIFESIGN_ON == true
unsigned long secondLast = 0;
void Display::lifeSign() {
  int color = ILI9341_GREEN;
  if (!sdCard.isReadyForLog()) {
    color = ILI9341_RED;
  }
  xSemaphoreTakeT(spiBus.mutex);
  tft.fillCircle(lifeSignX, lifeSignY, lifeSignRadius, lifeSignState ? ILI9341_DARKGREEN : color);
  xSemaphoreGive(spiBus.mutex);

  lifeSignState = !lifeSignState;

  unsigned long allSeconds = millis() / 1000;
  if (secondLast < allSeconds) {
    secondLast = allSeconds;
  }
}
#endif

void Display::drawCentreString(const string &buf, int x, int y) { return; }

// -------------
// FreeRTOS TASK
// -------------
void Display::task(void) {
  // polling loop
  while (1) {
    switch (carState.displayStatus) {
    // initializing states:
    case DISPLAY_STATUS::DRIVER_SETUP:
      bgColor = ILI9341_BLACK;
      carState.displayStatus = task(lifeSignCounter);
      break;
    case DISPLAY_STATUS::ENGINEER_SETUP:
      bgColor = ILI9341_ORANGE;
      carState.displayStatus = task(lifeSignCounter);
      break;
    case DISPLAY_STATUS::DRIVER_BACKGROUND:
      carState.displayStatus = task(lifeSignCounter);
      break;
    case DISPLAY_STATUS::ENGINEER_BACKGROUND:
      carState.displayStatus = task(lifeSignCounter);
      break;
    case DISPLAY_STATUS::DRIVER_DEMOSCREEN:
      carState.displayStatus = task(lifeSignCounter);
      break;
    // working states:
    case DISPLAY_STATUS::ENGINEER_CONSOLE:
      bgColor = ILI9341_WHITE;
      if (lifeSignCounter > 2) {
        lifeSign();
        lifeSignCounter = 0;
      }
      break;
    case DISPLAY_STATUS::ENGINEER_RUNNING:
      carState.displayStatus = task(lifeSignCounter);
      if (lifeSignCounter > 2) {
        lifeSign();
        lifeSignCounter = 0;
      }
      break;
    case DISPLAY_STATUS::DRIVER_RUNNING:
      carState.displayStatus = task(lifeSignCounter);
      if (lifeSignCounter > 1) {
        lifeSign();
        lifeSignCounter = 0;
      }
      break;
    case DISPLAY_STATUS::ENGINEER_HALTED:
      sleep_polling_ms = 1500;
      break;
#if WithTaskSuspend == true
      vTaskSuspend(getHandle());
#endif
      break;
    default:
      // ignore others
      break;
    }
#if LIFESIGN_ON == true
    lifeSignCounter++;
#endif
    vTaskDelay(sleep_polling_ms / portTICK_PERIOD_MS);
  }
}
