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
volatile INDICATOR curState = INDICATOR::OFF;

void update_indicator(int leftButton, int rightButton) {
  if (leftButton && rightButton) {
    if (curState == INDICATOR::WARN) {
      curState = INDICATOR::OFF;
    } else {
      curState = INDICATOR::WARN;
    }
  } else if (leftButton) {
    if (curState == INDICATOR::LEFT) {
      curState = INDICATOR::OFF;
    } else {
      curState = INDICATOR::LEFT;
    }
  } else if (rightButton) {
    if (curState == INDICATOR::RIGHT) {
      curState = INDICATOR::OFF;
    } else {
      curState = INDICATOR::RIGHT;
    }
  }
  indicator_set_and_blink(curState, true);
}

// ------------------
// FreeRTOS INIT TASK
// ------------------
void init_indicator(void) { vTaskDelay(1000 / portTICK_PERIOD_MS); }

// -------------
// FreeRTOS TASK
// -------------
void indicator_task(void *pvParameter) {
  // do not add code here -- only controlling the blink frequence
  // polling loop
  while (1) {
    indicator_set_and_blink(curState , blinkState);
    blinkState = !blinkState;

    // sleep
    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}
