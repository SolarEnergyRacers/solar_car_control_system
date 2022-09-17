//
// EngineeringDisplay
//
#include <iostream>

#include <LocalFunctionsAndDevices.h>
#include <abstract_task.h>
#include <definitions.h>

// #include <ADC.h>
#include <Display.h>
#include <EngineerDisplay.h>

#include <Adafruit_GFX.h>     // graphics library
#include <Adafruit_ILI9341.h> // display
#include <CarState.h>
#include <Console.h>

extern Console console;
extern SPIBus spiBus;
extern CarState carState;
extern Adafruit_ILI9341 tft;

DISPLAY_STATUS EngineerDisplay::display_setup() {
  console << "[v] '" << getName() << "' inited: screen D ILI9341 with " << height << " x " << width << "\n";
  return DISPLAY_STATUS::ENGINEER_BACKGROUND;
}

void EngineerDisplay::draw_display_background() {
  PhotoVoltaicOn.showLabel(tft);
  MotorOn.showLabel(tft);
  BatteryOn.showLabel(tft);
  Mppt1.showLabel(tft);
  Mppt2.showLabel(tft);
  Mppt3.showLabel(tft);
  // Mppt4.showLabel(tft);
  BatteryStatus.showLabel(tft);
  BmsStatus.showLabel(tft);
  Temperature1.showLabel(tft);
  Temperature2.showLabel(tft);
  Temperature3.showLabel(tft);
  //Temperature4.showLabel(tft);
  TemperatureMin.showLabel(tft);
  TemperatureMax.showLabel(tft);
  BatteryCurrent.showLabel(tft);
  BatteryVoltage.showLabel(tft);
  VoltageAvg.showLabel(tft);
  VoltageMin.showLabel(tft);
  VoltageMax.showLabel(tft);
}

// void EngineerDisplay::print(string msg) { Display::print(msg); }

DISPLAY_STATUS EngineerDisplay::task(int lifeSignCounter) {
  DISPLAY_STATUS status = carState.displayStatus;
  switch (carState.displayStatus) {
    // initializing states:
  case DISPLAY_STATUS::ENGINEER_SETUP:
    re_init();
    display_setup();
    justInited = true;
    status = DISPLAY_STATUS::ENGINEER_BACKGROUND;
    break;
  case DISPLAY_STATUS::ENGINEER_BACKGROUND:
    clear_screen(bgColor);
    draw_display_background();
    sleep_polling_ms = 500;
    status = DISPLAY_STATUS::ENGINEER_RUNNING;
    break;
  // working state:
  case DISPLAY_STATUS::ENGINEER_RUNNING:
    BatteryOn.Value = carState.BatteryOn;
    PhotoVoltaicOn.Value = carState.PhotoVoltaicOn;
    MotorOn.Value = carState.MotorOn;
    BatteryVoltage.Value = carState.BatteryVoltage;
    BatteryCurrent.Value = carState.BatteryCurrent;
    Temperature1.Value = carState.T1;
    Temperature2.Value = carState.T2;
    Temperature3.Value = carState.T3;
    //Temperature4.Value = carState.T4;
    TemperatureMin.Value = carState.Tmin;
    TemperatureMax.Value = carState.Tmax;
    Mppt1.Value = carState.Mppt1Current;
    Mppt2.Value = carState.Mppt2Current;
    Mppt3.Value = carState.Mppt3Current;

    VoltageMin.Value = carState.Umin;
    VoltageAvg.Value = carState.Uavg;
    VoltageMax.Value = carState.Umax;

    BatteryOn.showValue(tft);
    PhotoVoltaicOn.showValue(tft);
    MotorOn.showValue(tft);

    Mppt1.showValue(tft);
    Mppt2.showValue(tft);
    Mppt3.showValue(tft);
    // Mppt4.showValue(tft);

    BatteryStatus.showValue(tft);
    BmsStatus.showValue(tft);

    BatteryCurrent.showValue(tft);
    BatteryVoltage.showValue(tft);
    VoltageAvg.showValue(tft);
    VoltageMin.showValue(tft);
    VoltageMax.showValue(tft);

    Temperature1.showValue(tft);
    Temperature2.showValue(tft);
    Temperature3.showValue(tft);
    //Temperature4.showValue(tft);
    TemperatureMin.showValue(tft);
    TemperatureMax.showValue(tft);

    justInited = false;
    break;
  default:
    // ignore others
    break;
  }
  return status;
}
