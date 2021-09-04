//
// Display
//

#include <definitions.h>

#include <Adafruit_GFX.h>     // graphics library
#include <Adafruit_ILI9341.h> // display
#include <SPIBus.h>

#include <DriverDisplayC.h>
#include <Indicator.h>

INDICATOR Indicator::getIndicator() { return curState; }

void Indicator::setIndicator(INDICATOR state) {
  if (curState == state) {
    debug_printf("Set indicator '%d' off\n", static_cast<int>(state));
    curState = INDICATOR::OFF;
    // DriverDisplayC::indicator_set_and_blink(curState, false);
  } else {
    debug_printf("Set indicator '%d' on\n", static_cast<int>(state));
    curState = state;
    // DriverDisplayC::indicator_set_and_blink(curState, true);
  }
}

void Indicator::re_init() { init(); }

string Indicator::getName(void) { return "Indicator"; };

void Indicator::exit(void){
    // TODO
};

// ------------------
// FreeRTOS INIT TASK
// ------------------
void Indicator::init(void) {
  printf("[v] Indicator handler inited\n");
  vTaskDelay(1000 / portTICK_PERIOD_MS); // TODO: why sleep here?
  // return true; // TODO: restore functionality
}
// -------------
// FreeRTOS TASK
// -------------
void Indicator::task() {
  // do not add code here -- only controlling the blink frequency
  // polling loop
  DriverDisplayC *dd = DriverDisplayC::instance();
  while (1) {
    dd->indicator_set_and_blink(curState, blinkState);
    blinkState = !blinkState;

    // sleep
    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}
