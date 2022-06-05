//
// Display
//

#include <definitions.h>

#include <fmt/core.h>
#include <iostream>
#include <stdio.h>
#include <string>

#include <Console.h>
#include <Display.h>
#include <DriverDisplay.h>
#include <IOExt.h>
#include <Indicator.h>
#include <MCP23017.h>

extern CarState carState;
extern DriverDisplay driverDisplay;
extern Indicator indicator;
extern Console console;

// ------------------
// FreeRTOS functions
string Indicator::re_init() { return init(); }

string Indicator::init(void) {
  bool hasError = false;
  console << "[??] Init " + getName() + "...";
  console << "done.\n";
  return fmt::format("[{}] Indicator initialized.", hasError ? "--" : "ok");
}

void Indicator::exit(void){
    // TODO
};
// ------------------

void Indicator::setIndicator(INDICATOR state) {
  if (carState.Indicator == state) {
    console << fmt::format("Set indicator '{}' off\n", static_cast<int>(state));
    carState.Indicator = INDICATOR::OFF;
  } else {
    console << fmt::format("Set indicator '{}' on\n", static_cast<int>(state));
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

void Indicator::task() {
  // do not add code here -- only controlling the blink frequency
  // polling loop
  while (1) {

    // sleep
    vTaskDelay(sleep_polling_ms / portTICK_PERIOD_MS);
  }
}
