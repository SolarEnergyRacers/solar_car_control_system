//
// Display
//

#include "../../include/definitions.h"

#include <Adafruit_GFX.h>     // graphics library
#include <Adafruit_ILI9341.h> // display
#include <SPIBus.h>

#include "DriverDisplay.h"
#include "Indicator.h"

volatile bool blinkState = false;
volatile INDICATOR curState = INDICATOR_OFF;

INDICATOR getIndicator() { return curState; }

void setIndicator(INDICATOR state) {
  if (curState == state) {
    printf("Set indicator '%d' off\n", state);
    curState = INDICATOR_OFF;
    indicator_set_and_blink(curState, false);
  } else {
    printf("Set indicator '%d' on\n", state);
    curState = state;
    indicator_set_and_blink(curState, true);
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
  while (1) {
    indicator_set_and_blink(curState, blinkState);
    blinkState = !blinkState;

    // sleep
    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}
