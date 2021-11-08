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

#include <CarState.h>
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
extern bool systemOk;
extern CarState carState;

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

void Display::init() {
  abstract_task::init();
  printf("\n");
  re_init();
}

void Display::re_init(void) {
#if WithTaskSuspend == true
  vTaskResume(getHandle());
#endif
  _setup();
}

// Adafruit_ILI9341 Display::tft = Adafruit_ILI9341(SPI_CS_TFT, SPI_DC, SPI_RST);
Adafruit_ILI9341 Display::tft = Adafruit_ILI9341(SPI_CS_TFT, SPI_DC, SPI_MOSI, SPI_CLK, SPI_RST, SPI_MISO);

void Display::_setup() {
  printf("    Setup 'ILI9341' for '%s' with: SPI_CLK=%d, SPI_MOSI=%d, SPI_MISO=%d, SPI_CS_TFT=%d.\n", getName().c_str(), SPI_CLK, SPI_MOSI,
         SPI_MISO, SPI_CS_TFT);
  height = 0;
  width = 0;
  try {
    uint8_t rdmode = 0;
    uint8_t rdmadctl = 0;
    uint8_t rdpixfmt = 0;
    uint8_t rdimgfmt = 0;
    uint8_t rdselfdiag = 0;

    xSemaphoreTakeT(spiBus.mutex);
    //tft = Adafruit_ILI9341(SPI_CS_TFT, SPI_DC, SPI_MOSI, SPI_CLK, SPI_RST, SPI_MISO);
    tft.begin();
    tft.setRotation(1);
    height = tft.height();
    width = tft.width();
    rdmode = tft.readcommand8(ILI9341_RDMODE);
    rdmadctl = tft.readcommand8(ILI9341_RDMADCTL);
    rdpixfmt = tft.readcommand8(ILI9341_RDPIXFMT);
    rdimgfmt = tft.readcommand8(ILI9341_RDIMGFMT);
    rdselfdiag = tft.readcommand8(ILI9341_RDSELFDIAG);
    tft.setCursor(0,0);
    tft.setTextSize(1);
    tft.fillScreen(bgColor);
    tft.setTextColor(ILI9341_BLUE);
    tft.setScrollMargins(0, height);
    tft.printf("%s inited (%dx%d)-->%s\n", getName().c_str(), height, width, DISPLAY_STATUS_str[(int)status]);
    xSemaphoreGive(spiBus.mutex);
    printf("    ILI9341_RDMADCTL:   0x%x\n", rdmadctl);
    printf("    ILI9341_RDPIXFMT:   0x%x\n", rdpixfmt);
    printf("    ILI9341_RDIMGFMT:   0x%x\n", rdimgfmt);
    printf("    ILI9341_RDSELFDIAG: 0x%x\n", rdselfdiag);
    printf("    ILI9341_RDMODE:     0x%x\n", rdmode);
  } catch (__exception ex) {
    printf("[x] Display: Unable to initialize screen 'ILI9341'.\n");
    xSemaphoreGive(spiBus.mutex);
    throw ex;
  }
  lifeSignX = width - lifeSignRadius - 6;
  lifeSignY = height - lifeSignRadius - 6;
  status = DISPLAY_STATUS::CONSOLE;
  printf("[v] '%s' Display inited: screen 'ILI9341' with %d x %d. Status: %s\n", getName().c_str(), height, width,
         DISPLAY_STATUS_str[(int)status]);
}

void Display::clear_screen(int bgColor) {
  xSemaphoreTakeT(spiBus.mutex);
  tft.fillScreen(bgColor);
  xSemaphoreGive(spiBus.mutex);
}

void Display::exit() {}

int Display::getPixelWidthOfTexts(int textSize, string t1, string t2) {
  int l1 = t1.length() * textSize * 6;
  int l2 = t2.length() * textSize * 6;
  return max(l1, l2);
}

void Display::print(string msg) {
  if (!_is_ready())
    return;
  if (status == DISPLAY_STATUS::CONSOLE) {
    xSemaphoreTakeT(spiBus.mutex);
    tft.setTextSize(1);
    tft.print(msg.c_str());
    xSemaphoreGive(spiBus.mutex);
  }
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
  debug_printf("ERROR: Semaphore not found!%s", "\n");
}

int Display::scroll(int lines) {
  if (!_is_ready())
    return 0;
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
  if (_is_ready() && xSemaphoreTake(spiBus.mutex, portMAX_DELAY) == pdTRUE) {
    tft.writeCommand(ILI9341_VSCRSADD); // Vertical scrolling start address
    tft.write(VSP >> 8);
    tft.write(VSP);
    xSemaphoreGive(spiBus.mutex);
  }
}

//________________________________________________________________________

void exit(void) {}

// writes Display::float value  in the range from -9999.9 to 9999.9
float Display::write_float(int x, int y, float valueLast, float value, int textSize, int color) {
  if (!_is_ready())
    return value;
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
int Display::write_ganz_99(int x, int y, int valueLast, int value, int textSize, int color) {
  if (!_is_ready())
    return value;
  if (value < -99 || value > 999) {
    printf("ERROR: call write_ganz_99 with a value outside the range: '%d'", value);
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
int Display::write_nat_999(int x, int y, int valueLast, int value, int textSize, int color) {
  if (!_is_ready())
    return value;
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
void Display::lifeSign() {
  if (!_is_ready())
    return;
  xSemaphoreTakeT(spiBus.mutex);
  tft.fillCircle(lifeSignX, lifeSignY, lifeSignRadius, lifeSignState ? ILI9341_DARKGREEN : ILI9341_GREEN);
  xSemaphoreGive(spiBus.mutex);

  lifeSignState = !lifeSignState;
}

void Display::drawCentreString(const string &buf, int x, int y) {
  if (!_is_ready())
    return;
  // xSemaphoreTakeT(spiBus.mutex);

  // int16_t x1, y1;
  // uint16_t w, h;
  // tft.getTextBounds(buf, &x, &y, &x1, &y1, &w, &h); // calc width of new string
  // tft.setCursor(x - w / 2, y);
  // tft.print(buf.c_str());

  // xSemaphoreGive(spiBus.mutex);}
}

// -------------
// FreeRTOS TASK
// -------------
void Display::task(void) {
  // polling loop
  while (1) {
    switch (status) {
    // initializing states:
    case DISPLAY_STATUS::SETUPDRIVER:
      status = task(status, lifeSignCounter);
      break;
    case DISPLAY_STATUS::SETUPENGINEER:
      bgColor = ILI9341_ORANGE;
      status = task(status, lifeSignCounter);
      break;
    case DISPLAY_STATUS::BACKGROUNDDRIVER:
      status = task(status, lifeSignCounter);
      break;
    case DISPLAY_STATUS::BACKGROUNDENGINEER:
      status = task(status, lifeSignCounter);
      break;
    case DISPLAY_STATUS::DEMOSCREEN:
      status = task(status, lifeSignCounter);
      break;
    // working states:
    case DISPLAY_STATUS::CONSOLE:
      if (lifeSignCounter > 2) {
        lifeSign();
        lifeSignCounter = 0;
      }
      break;
    case DISPLAY_STATUS::ENGINEER:
      status = task(status, lifeSignCounter);
      if (lifeSignCounter > 2) {
        lifeSign();
        lifeSignCounter = 0;
      }
      break;
    case DISPLAY_STATUS::DRIVER:
      status = task(status, lifeSignCounter);
      if (lifeSignCounter > 1) {
        lifeSign();
        lifeSignCounter = 0;
      }
      break;
    case DISPLAY_STATUS::HALTED:
      set_SleepTime(1500);
      break;
#if WithTaskSuspend == true
      vTaskSuspend(getHandle());
#endif
      break;
    default:
      // ignore others
      break;
    }
    lifeSignCounter++;
    // debug_printf("%16s -- sleep_polling_ms: %d\n", getName().c_str(), sleep_polling_ms);
    vTaskDelay(sleep_polling_ms / portTICK_PERIOD_MS);
  }
}

// } //namespace Display
