//
// Display
//

#include <definitions.h>

#include <fmt/core.h>
#include <iostream>
#include <stdio.h>
#include <string>

#include <Display.h>
#include <DriverDisplay.h>
#include <IOExt2.h>
#include <Indicator.h>
#include <MCP23017.h>

extern CarState carState;
extern DriverDisplay driverDisplay;
extern Indicator indicator;

// ------------------
// FreeRTOS functions
string Indicator::getName(void) { return "Indicator"; };

void Indicator::re_init() { init(); }

void Indicator::init(void) {
  set_SleepTime(200);
  string s = "[v] " + getName() + " initialized.\n";
  cout <<s;
  driverDisplay.print(s.c_str());
}

void Indicator::exit(void){
    // TODO
};
// ------------------

void Indicator::setIndicator(INDICATOR state) {
  if (carState.Indicator == state) {
    debug_printf("Set indicator '%d' off\n", static_cast<int>(state));
    carState.Indicator = INDICATOR::OFF;
  } else {
    debug_printf("Set indicator '%d' on\n", static_cast<int>(state));
    carState.Indicator = state;
  }
}

// pins are low active
void Indicator::setIndicator(int left, int right) {
  if (left == 0 && right == 0)
    setIndicator(INDICATOR::WARN);
  else if (left == 0)
    setIndicator(INDICATOR::LEFT);
  else if (right == 0)
    setIndicator(INDICATOR::RIGHT);
}

bool Indicator::getIndicatorLeft() {
  return (carState.Indicator == INDICATOR::LEFT || carState.Indicator == INDICATOR::WARN) && carState.IndicatorBlink;
}

bool Indicator::getIndicatorRight() {
  return (carState.Indicator == INDICATOR::RIGHT || carState.Indicator == INDICATOR::WARN) && carState.IndicatorBlink;
}

unsigned long lastFlip = 0;
bool justSwitchedOff = true;
void Indicator::task() {
  // do not add code here -- only controlling the blink frequency
  // polling loop
  while (1) {
    if (carState.Indicator != INDICATOR::OFF) {
      justSwitchedOff = true;
      if (carState.IndicatorBlink && (millis() - lastFlip) > intervall_on) {
        lastFlip = millis();
        carState.IndicatorBlink = false;
      } else if (!carState.IndicatorBlink && (millis() - lastFlip) > intervall_off) {
        lastFlip = millis();
        carState.IndicatorBlink = true;
      }
      carState.getPin(PinIndicatorOutLeft)->value = indicator.getIndicatorLeft();
      carState.getPin(PinIndicatorOutRight)->value = indicator.getIndicatorRight();
    } else if (justSwitchedOff) {
      carState.getPin(PinIndicatorOutLeft)->value = 0;
      carState.getPin(PinIndicatorOutRight)->value = 0;
      justSwitchedOff = false;
    }

    // sleep
    vTaskDelay(sleep_polling_ms / portTICK_PERIOD_MS);
  }
}
