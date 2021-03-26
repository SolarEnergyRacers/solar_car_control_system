//
// Display
//

#include "../../include/definitions.h"

#include <SPIBus.h>
#include "Simulator.h"
#include "Indicator.h"
#include "DriverDisplay.h"

#include <Adafruit_GFX.h>     // graphics library
#include <Adafruit_ILI9341.h> // display

// ------------------
// FreeRTOS INIT TASK
// ------------------
void init_simulator(void)
{

    // // CRITICAL SECTION SPI: start
    // xSemaphoreTake(simulator_mutex, portMAX_DELAY);

    // xSemaphoreGive(simulator_mutex);
    // // CRITICAL SECTION SPI: end

    vTaskDelay(1000 / portTICK_PERIOD_MS);
}

// -------------
// FreeRTOS TASK
// -------------
void simulator_task(void *pvParameter)
{
    // do not add code here -- only controlling the blink frequence
    // polling loop
    while (1)
    {
        // // CRITICAL SECTION SIMULATOR: start
        // xSemaphoreTake(simulator_mutex, portMAX_DELAY);


        // xSemaphoreGive(simulator_mutex);
        // // CRITICAL SECTION SIMULATOR: end

        // sleep for 1s
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
