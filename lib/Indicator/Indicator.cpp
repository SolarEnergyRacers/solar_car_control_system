//
// Display
//

#include "../../include/definitions.h"

#include <SPIBus.h>
#include <Adafruit_GFX.h>     // graphics library
#include <Adafruit_ILI9341.h> // display

#include "DriverDisplay.h"
#include "Indicator.h"

bool blinkState = false;
INDICATOR direction = INDICATOR::OFF;

void update_indicator(int leftButton, int rightButton)
{
    if (leftButton && rightButton)
    {
        if (direction == INDICATOR::WARN)
        {
            direction = INDICATOR::OFF;
        }
        else
        {
            direction = INDICATOR::WARN;
        }
    }
    else if (leftButton)
    {
        if (direction == INDICATOR::LEFT)
        {
            direction = INDICATOR::OFF;
        }
        else
        {
            direction = INDICATOR::LEFT;
        }
    }
    else if (rightButton)
    {
        if (direction == INDICATOR::RIGHT)
        {
            direction = INDICATOR::OFF;
        }
        else
        {
            direction = INDICATOR::RIGHT;
        }
    }
    indicator_set_and_blink(direction, true);
}

// ------------------
// FreeRTOS INIT TASK
// ------------------
void init_indicator(void)
{

    vTaskDelay(1000 / portTICK_PERIOD_MS);
}

// -------------
// FreeRTOS TASK
// -------------
void indicator_task(void *pvParameter)
{
    // do not add code here -- only controlling the blink frequence
    // polling loop
    while (1)
    {
        indicator_set_and_blink(direction, blinkState);
        blinkState = !blinkState;

        // sleep
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}
