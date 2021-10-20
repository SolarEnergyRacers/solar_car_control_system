//
// Display
//

#include <LocalFunctionsAndDevices.h>
#include <abstract_task.h>
#include <definitions.h>

#include <ADC.h>
#include <Display.h>
#include <EngineerDisplay.h>

#include <Adafruit_GFX.h>     // graphics library
#include <Adafruit_ILI9341.h> // display
#include <CarState.h>
#include <SPIBus.h>

extern SPIBus spiBus;
extern ADC adc;
extern CarState carState;
extern Adafruit_ILI9341 tft;

void EngineerDisplay::draw_display_background() {

  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
  tft.setRotation(1);
  // tft.setTextSize(2);
  // tft.setTextColor(ILI9341_DARKGREEN);
  xSemaphoreGive(spiBus.mutex);

  PvStatus.showLabel(tft);
  McStatus.showLabel(tft);
  Mppt1.showLabel(tft);
  Mppt2.showLabel(tft);
  Mppt3.showLabel(tft);
  Mppt4.showLabel(tft);
  BatteryStatus.showLabel(tft);
  BmsStatus.showLabel(tft);
  Temperature1.showLabel(tft);
  Temperature2.showLabel(tft);
  Temperature3.showLabel(tft);
  Temperature4.showLabel(tft);
  TemperatureMax.showLabel(tft);
  BatteryCurrent.showLabel(tft);
  BatteryVoltage.showLabel(tft);
  VoltageAvg.showLabel(tft);
  VoltageMin.showLabel(tft);
  VoltageMax.showLabel(tft);

  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
  tft.setTextSize(1);
  xSemaphoreGive(spiBus.mutex);
}

DISPLAY_STATUS EngineerDisplay::display_setup(DISPLAY_STATUS status) {
  printf("    Init 'EngineerDisplay'\n");
  int height = 0;
  int width = 0;
  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
  try {
    height = tft.height();
    width = tft.width();
    tft.fillScreen(ILI9341_LIGHTGREY);
    tft.setRotation(1);
    tft.setTextSize(1);
    tft.setTextColor(ILI9341_DARKGREEN);
    tft.setScrollMargins(10, width - 20);

  } catch (__exception ex) {
    printf("[x] EngineerDisplay: Unable to initialize screen ILI9341.\n");
    throw ex;
  }
  xSemaphoreGive(spiBus.mutex);
  printf("[v] EngineerDisplay inited: screen ILI9341 with %d x %d.\n", height, width);
  return DISPLAY_STATUS::BACKGROUNDENGINEER;
}

DISPLAY_STATUS EngineerDisplay::task(DISPLAY_STATUS status, int lifeSignCounter) {
  switch (status) {
    // initializing states:
  case DISPLAY_STATUS::SETUPENGINEER:
    display_setup(status);
    clear_screen(bgColor);
    status = DISPLAY_STATUS::BACKGROUNDENGINEER;
    debug_printf("DISPLAY_STATUS-E::%s\n", DISPLAY_STATUS_str[(int)status]);
    break;
  case DISPLAY_STATUS::BACKGROUNDENGINEER:
    draw_display_background();
    status = DISPLAY_STATUS::ENGINEER;
    debug_printf("DISPLAY_STATUS-E::%s\n", DISPLAY_STATUS_str[(int)status]);
    break;
  // working state:
  case DISPLAY_STATUS::ENGINEER:
    if (lifeSignCounter > 10) {
      PvStatus.Value = carState.PhotoVoltaic.get();
      PvStatus.showValue(tft);
      McStatus.Value = carState.ConstantModeOn.get();
      McStatus.showValue(tft);
    }
  default:
    // ignore others
    break;
  }
  return status;
}

void EngineerDisplay::print(string msg) { Display::print(msg); }
