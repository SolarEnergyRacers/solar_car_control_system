//
// Display
//

#include "../../include/definitions.h"

#include <SPIBus.h>
#include "DriverDisplay.h"
#include "Indicator.h"

#include <Adafruit_GFX.h>     // graphics library
#include <Adafruit_ILI9341.h> // display

// set indicator [l - left, r - right, w - hazard warning, o - off]
void indicator(char direction)
{
    setIndicatorDirection(direction);
}

// ------------------
// FreeRTOS INIT TASK
// ------------------
void init_indicator(void)
{

    // CRITICAL SECTION SPI: start
    xSemaphoreTake(spi_mutex, portMAX_DELAY);

    xSemaphoreGive(spi_mutex);
    // CRITICAL SECTION SPI: end

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

        // CRITICAL SECTION SPI: start
        xSemaphoreTake(spi_mutex, portMAX_DELAY);

        if (getIndicatorState())
        {
            turn_indicator(getIndicatorDirection());
        }
        else
        {
            turn_indicator('o');
        }
        setIndicatorState(!getIndicatorState());

        xSemaphoreGive(spi_mutex);
        // CRITICAL SECTION SPI: end

        // sleep for 1s
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}
