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

DISPLAY_STATUS EngineerDisplay::display_setup(DISPLAY_STATUS status) {
  printf("    Init 'EngineerDisplay'\n");
  int height = 0;
  int width = 0;
  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
  try {
    height = tft.height();
    width = tft.width();
    tft.setRotation(1);
    tft.setTextSize(1);
    tft.setTextColor(ILI9341_DARKGREEN);
    tft.setScrollMargins(10, width - 20);

  } catch (__exception ex) {
    xSemaphoreGive(spiBus.mutex);
    printf("[x] EngineerDisplay: Unable to initialize screen ILI9341.\n");
    throw ex;
  }
  xSemaphoreGive(spiBus.mutex);
  printf("[v] EngineerDisplay inited: screen ILI9341 with %d x %d.\n", height, width);
  return DISPLAY_STATUS::BACKGROUNDENGINEER;
}

void EngineerDisplay::draw_display_background() {
  xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
  tft.setRotation(1);
  xSemaphoreGive(spiBus.mutex);

  PhotoVoltaicOn.showLabel(tft);
  MotorOn.showLabel(tft);
  BatteryOn.showLabel(tft);
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

void EngineerDisplay::print(string msg) { Display::print(msg); }

DISPLAY_STATUS EngineerDisplay::task(DISPLAY_STATUS status, int lifeSignCounter) {
  switch (status) {
    // initializing states:
  case DISPLAY_STATUS::SETUPENGINEER:
    display_setup(status);
    status = DISPLAY_STATUS::BACKGROUNDENGINEER;
    debug_printf("DISPLAY_STATUS-E::%s\n", DISPLAY_STATUS_str[(int)status]);
    break;
  case DISPLAY_STATUS::BACKGROUNDENGINEER:
    clear_screen(bgColor);
    draw_display_background();
    status = DISPLAY_STATUS::ENGINEER;
    debug_printf("DISPLAY_STATUS-E::%s\n", DISPLAY_STATUS_str[(int)status]);
    break;
  // working state:
  case DISPLAY_STATUS::ENGINEER:
    if (lifeSignCounter > 10) {
      PhotoVoltaicOn.Value = carState.PhotoVoltaicOn.get();
      MotorOn.Value = carState.MotorOn.get();
      BatteryOn.Value = carState.BatteryOn.get();
      BatteryVoltage.Value = carState.BatteryVoltage.get();
      BatteryCurrent.Value = carState.BatteryCurrent.get();

      PhotoVoltaicOn.showValue(tft);
      MotorOn.showValue(tft);
      BatteryOn.showValue(tft);
      Mppt1.showValue(tft);
      Mppt2.showValue(tft);
      Mppt3.showValue(tft);
      Mppt4.showValue(tft);
      BatteryStatus.showValue(tft);
      BmsStatus.showValue(tft);
      Temperature1.showValue(tft);
      Temperature2.showValue(tft);
      Temperature3.showValue(tft);
      Temperature4.showValue(tft);
      TemperatureMax.showValue(tft);
      BatteryCurrent.showValue(tft);
      BatteryVoltage.showValue(tft);
      VoltageAvg.showValue(tft);
      VoltageMin.showValue(tft);
      VoltageMax.showValue(tft);
    }
  default:
    // ignore others
    break;
  }
  return status;
}
