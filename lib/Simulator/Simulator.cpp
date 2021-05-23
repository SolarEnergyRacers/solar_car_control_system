//
// Display
//

#include "Simulator.h"
#include "../../include/definitions.h"
#include "DriverDisplay.h"
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

// counters for demo display
int counterIndicator = 0;
int counterSpeed = 0;
int counterPV = 0;

// ------------------
// FreeRTOS INIT TASK
// ------------------
void init_simulator(void)
{
    driver_display_demo_screen();
    printf("[v] Simulator inited.\n");
    delay(10000);

    vTaskDelay(1000 / portTICK_PERIOD_MS);
}

// -------------
// FreeRTOS TASK
// -------------
void simulator_task(void *pvParameter)
{
    // polling loop
    while (1)
    {
        // CRITICAL SECTION SPI: start
        xSemaphoreTake(spi_mutex, portMAX_DELAY);
        switch (counterIndicator++)
        {
        case 0:
            write_driver_info("Stop!", INFO_TYPE_ERROR);
            arrow_increase(false);
            arrow_decrease(false);
            break;
        case 40:
            write_driver_info("Go", INFO_TYPE_INFO);
            //write_driver_info("0123456789ABCDEF0123456789", ILI9341_WHITE);
            arrow_increase(true);
            break;
        case 80:
            write_driver_info("Go", INFO_TYPE_INFO);
            arrow_decrease(true);
            break;
        case 120:
            write_driver_info("", INFO_TYPE_INFO);
            arrow_increase(false);
            arrow_decrease(false);
            break;
        case 170:
            counterIndicator = 0;
        }
    switch (counterSpeed++) {
    case 0:
      draw_display_border(ILI9341_GREEN);
      write_speed(1);
      break;
    case 1:
      write_speed(12);
      break;
    case 2:
      write_speed(123);
      break;
    case 3:
      draw_display_border(ILI9341_YELLOW);
      write_speed(888);
      delay(1000);
      break;
    case 4:
      draw_display_border(ILI9341_RED);
      write_speed(120);
      break;
    case 5:
      write_speed(42);
      break;
    case 6:
      write_speed(0);
      draw_display_border(ILI9341_RED);
      break;
    default:
      write_speed(counterSpeed);
      draw_display_border(ILI9341_GREEN);
      if (counterSpeed > 990) {
        counterSpeed = 0;
      }
    }

    switch (counterPV++) {
    case 0:
      write_pv(1);
      write_motor(1.1);
      break;
    case 1:
      write_pv(12);
      write_motor(12.3);
      break;
    case 2:
      write_pv(123);
      write_motor(123.4);
      break;
    case 3:
      write_pv(1234);
      write_motor(1234.5);
      break;
    case 4:
      write_pv(-1);
      write_motor(-1.2);
      break;
    case 5:
      write_pv(-12);
      write_motor(-12.3);
      break;
    case 6:
      write_pv(-123);
      write_motor(-123.4);
      counterPV = -99999;
      break;
    default:
      write_pv((float)counterPV / 10);
      write_motor((float)counterPV / -10);
      write_bat((float)counterPV / 1000);
      if (counterPV > 99999) {
        counterPV = -99999;
      }
    }

    // tft.setRotation(1);
    // tft.setCursor(0, 0);
    // tft.setTextColor(ILI9341_WHITE);
    // tft.setTextSize(1);
    // tft.println("Hello ILI!");

    // // clears the screen and buffer
    // tft.fillScreen(ILI9341_BLACK);

    // // setup params
    // tft.setTextSize(1);
    // tft.setTextWrap(false);
    // tft.setTextColor(ILI9341_GREENYELLOW);
    // tft.setCursor(0, 0);

    // // print demo characters
    // for (int i = 0; i < 168; i++)
    // {
    //     if (i == '\n')
    //         continue;
    //     // write char
    //     tft.write(i);
    //     // newline
    //     if ((i > 0) && (i % 21 == 0))
    //         tft.println();
    // }
    // //tft.display();

    xSemaphoreGive(spi_mutex);
    // CRITICAL SECTION SPI: end

    // sleep for 1s
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
