//
// Display
//

#include "../../include/definitions.h"

#include <Adafruit_GFX.h>     // graphics library
#include <Adafruit_ILI9341.h> // display
#include <SPIBus.h>

#include "DriverDisplayC.h"
#include "Indicator.h"

volatile bool blinkState = false;
volatile INDICATOR curState = INDICATOR::OFF;

INDICATOR getIndicator() { return curState; }

void setIndicator(INDICATOR state) {
  if (curState == state) {
    printf("Set indicator '%d' off\n", static_cast<int>(state));
    curState = INDICATOR::OFF;
    //DriverDisplayC::indicator_set_and_blink(curState, false);
  } else {
    printf("Set indicator '%d' on\n", static_cast<int>(state));
    curState = state;
    //DriverDisplayC::indicator_set_and_blink(curState, true);
  }
}

// ------------------
// FreeRTOS INIT TASK
// ------------------
bool init_indicator(void) {
  printf("[v] Indicator handler inited\n");
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  return true;
}
// -------------
// FreeRTOS TASK
// -------------
void indicator_task(void *pvParameter) {
  // do not add code here -- only controlling the blink frequence
  // polling loop
  DriverDisplayC *dd = DriverDisplayC::instance();
  while (1) {
    dd->indicator_set_and_blink(curState, blinkState);
    blinkState = !blinkState;

    // sleep
    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}
