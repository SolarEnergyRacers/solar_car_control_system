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

// Adafruit_ILI9341 tft = Adafruit_ILI9341(0, 0, 0, 0, 0, 0);
// namespace Display {

//==== Display definitions ==== START
int bgColor = ILI9341_WHITE;
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

DISPLAY_STATUS Display::status = DISPLAY_STATUS::CONSOLE;

string Display::getName() { return "Display"; };

void Display::init() {
  abstract_task::init();
  printf("\n");
  re_init();
  status = DISPLAY_STATUS::CONSOLE;
}

void Display::re_init(void) {
  _setup();
  status = DISPLAY_STATUS::CONSOLE;
}

void Display::_setup() {
  printf("    Setup 'ILI9341' with: SPI_CLK=%d, SPI_MOSI=%d, SPI_MISO=%d, SPI_CS_TFT=%d.\n", SPI_CLK, SPI_MOSI, SPI_MISO, SPI_CS_TFT);
  int height = 0;
  int width = 0;
  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
  tft = Adafruit_ILI9341(SPI_CS_TFT, SPI_DC, SPI_MOSI, SPI_CLK, SPI_RST, SPI_MISO);
  tft.begin();
  try {
    height = tft.height();
    width = tft.width();
    tft.setRotation(1);
    tft.setTextSize(1);
    tft.fillScreen(ILI9341_WHITE);
    tft.setTextColor(ILI9341_BLUE);
    tft.setScrollMargins(0, height);
    uint8_t x = tft.readcommand8(ILI9341_RDMODE);
    printf("    Display Power Mode: 0x%x\n", x);
    x = tft.readcommand8(ILI9341_RDMADCTL);
    printf("    MADCTL Mode:        0x%x\n", x);
    x = tft.readcommand8(ILI9341_RDPIXFMT);
    printf("    Pixel Format:       0x%x\n", x);
    x = tft.readcommand8(ILI9341_RDIMGFMT);
    printf("    Image Format:       0x%x\n", x);
    x = tft.readcommand8(ILI9341_RDSELFDIAG);
    printf("    Self Diagnostic:    0x%x\n", x);
    lifeSignX = tft.width() - lifeSignRadius - 6;
    lifeSignY = tft.height() - lifeSignRadius - 6;
    tft.printf("Screen ILI9341 inited with %d x %d.\n", height, width);
  } catch (__exception ex) {
    printf("[x] Display: Unable to initialize screen 'ILI9341'.\n");
    xSemaphoreGive(spiBus.mutex);
    throw ex;
  }
  xSemaphoreGive(spiBus.mutex);
  status = DISPLAY_STATUS::CONSOLE;
  printf("[v] Display inited: screen 'ILI9341' with %d x %d.\n", height, width);
}

void Display::clear_screen(int bgColor) {
  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
  tft.fillScreen(bgColor);
  xSemaphoreGive(spiBus.mutex);
}

void Display::exit() {}

DISPLAY_STATUS Display::display_setup(DISPLAY_STATUS status) { return status; }

DISPLAY_STATUS Display::task(DISPLAY_STATUS status, int lifeSignCounter) { return status; }

// -------------
// FreeRTOS TASK
// -------------
void Display::task(void) {
  // polling loop
  while (1) {
    switch (status) {
    // initializing states:
    case DISPLAY_STATUS::SETUPDRIVER:
    case DISPLAY_STATUS::SETUPENGINEER:
      status = task(status, lifeSignCounter);
      break;
    case DISPLAY_STATUS::DEMOSCREEN:
      status = task(status, lifeSignCounter);
      break;
    case DISPLAY_STATUS::BACKGROUNDDRIVER:
    case DISPLAY_STATUS::BACKGROUNDENGINEER:
      status = task(status, lifeSignCounter);
      break;
    // working states:
    case DISPLAY_STATUS::CONSOLE:
      if (lifeSignCounter > 40) {
        lifeSign();
        lifeSignCounter = 0;
        // status = task(status, lifeSignCounter);
      }
      break;
    case DISPLAY_STATUS::ENGINEER:
      if (lifeSignCounter > 40) {
        status = task(status, lifeSignCounter);
        lifeSign();
        lifeSignCounter = 0;
      }
      break;
    case DISPLAY_STATUS::DRIVER:
    default:
      if (lifeSignCounter > 20) {
        status = task(status, lifeSignCounter);
        lifeSign();
        lifeSignCounter = 0;
      }
      break;
    }
    lifeSignCounter++;
    // sleep for sleep_polling_ms
    this->sleep(20);
  }
}

int Display::getPixelWidthOfTexts(int textSize, string t1, string t2) {
  int l1 = t1.length() * textSize * 6;
  int l2 = t2.length() * textSize * 6;
  return max(l1, l2);
}

void Display::print(string msg) {
  if (status == DISPLAY_STATUS::CONSOLE) {
    xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
    tft.setTextSize(1);
    tft.print(msg.c_str());
    xSemaphoreGive(spiBus.mutex);
  }
}

//------------------------------------------------------------------------
void Display::setupScrollArea(uint16_t TFA, uint16_t BFA) {
  tft.writeCommand(ILI9341_VSCRDEF); // Vertical scroll definition
  tft.write(TFA >> 8);
  tft.write(TFA);
  tft.write((320 - TFA - BFA) >> 8);
  tft.write(320 - TFA - BFA);
  tft.write(BFA >> 8);
  tft.write(BFA);
}

int Display::scroll(int lines) {
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

void Display::scrollAddress(uint16_t VSP) {
  tft.writeCommand(ILI9341_VSCRSADD); // Vertical scrolling start address
  tft.write(VSP >> 8);
  tft.write(VSP);
}

//________________________________________________________________________

void exit(void) {}

// writes Display::float value  in the range from -9999.9 to 9999.9
float Display::write_float(int x, int y, float valueLast, float value, int textSize, int color) {

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
  tft.setTextSize(1);
  xSemaphoreGive(spiBus.mutex);

  return value;
}

// writes integer value in the range from -99 to +99
int Display::write_ganz_99(int x, int y, int valueLast, int value, int textSize, int color) {
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
  tft.setTextSize(1);
  xSemaphoreGive(spiBus.mutex);

  return value;
}

// writes integer value in the range from 0 to 999
int Display::write_nat_999(int x, int y, int valueLast, int value, int textSize, int color) {
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
void Display::lifeSign() {
  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
  tft.fillCircle(lifeSignX, lifeSignY, lifeSignRadius, lifeSignState?ILI9341_DARKGREEN:ILI9341_GREEN);
  xSemaphoreGive(spiBus.mutex);

  lifeSignState = !lifeSignState;
}

void Display::drawCentreString(const string &buf, int x, int y) {
  // xSemaphoreTake(spiBus.mutex, portMAX_DELAY);

  // int16_t x1, y1;
  // uint16_t w, h;
  // tft.getTextBounds(buf, &x, &y, &x1, &y1, &w, &h); // calc width of new string
  // tft.setCursor(x - w / 2, y);
  // tft.print(buf.c_str());

  // xSemaphoreGive(spiBus.mutex);
}

int Display::getColorForInfoType(INFO_TYPE type) {
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

// } //namespace Display
