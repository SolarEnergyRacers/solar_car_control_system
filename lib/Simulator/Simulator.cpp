//
// Display
//

#include "Simulator.h"
#include "../../include/definitions.h"
#include "DriverDisplayC.h"
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
void init_simulator(void) {
  DriverDisplayC *dd = DriverDisplayC::instance();

  dd->driver_display_demo_screen();
  printf("[v] Simulator inited.\n");
  delay(10000);

  vTaskDelay(1000 / portTICK_PERIOD_MS);
}

// -------------
// FreeRTOS TASK
// -------------
void simulator_task(void *pvParameter) {
  // polling loop
  DriverDisplayC *dd = DriverDisplayC::instance();

  while (1) {
    // CRITICAL SECTION SPI: start
    xSemaphoreTake(spi_mutex, portMAX_DELAY);
    switch (counterIndicator++) {
    case 0:
      dd->write_driver_info("Stop!", INFO_TYPE::ERROR);
      dd->arrow_increase(false);
      dd->arrow_decrease(false);
      break;
    case 40:
      dd->write_driver_info("Go", INFO_TYPE::INFO);
      // write_driver_info("0123456789ABCDEF0123456789", ILI9341_WHITE);
      dd->arrow_increase(true);
      break;
    case 80:
      dd->write_driver_info("Go", INFO_TYPE::INFO);
      dd->arrow_decrease(true);
      break;
    case 120:
      dd->write_driver_info("", INFO_TYPE::INFO);
      dd->arrow_increase(false);
      dd->arrow_decrease(false);
      break;
    case 170:
      counterIndicator = 0;
    }
    switch (counterSpeed++) {
    case 0:
      dd->draw_display_border(ILI9341_GREEN);
      dd->write_speed(1);
      break;
    case 1:
      dd->write_speed(12);
      break;
    case 2:
      dd->write_speed(123);
      break;
    case 3:
      dd->draw_display_border(ILI9341_YELLOW);
      dd->write_speed(888);
      delay(1000);
      break;
    case 4:
      dd->draw_display_border(ILI9341_RED);
      dd->write_speed(120);
      break;
    case 5:
      dd->write_speed(42);
      break;
    case 6:
      dd->write_speed(0);
      dd->draw_display_border(ILI9341_RED);
      break;
    default:
      dd->write_speed(counterSpeed);
      dd->draw_display_border(ILI9341_GREEN);
      if (counterSpeed > 990) {
        counterSpeed = 0;
      }
    }

    switch (counterPV++) {
    case 0:
      dd->write_pv(1);
      dd->write_motor(1.1);
      break;
    case 1:
      dd->write_pv(12);
      dd->write_motor(12.3);
      break;
    case 2:
      dd->write_pv(123);
      dd->write_motor(123.4);
      break;
    case 3:
      dd->write_pv(1234);
      dd->write_motor(1234.5);
      break;
    case 4:
      dd->write_pv(-1);
      dd->write_motor(-1.2);
      break;
    case 5:
      dd->write_pv(-12);
      dd->write_motor(-12.3);
      break;
    case 6:
      dd->write_pv(-123);
      dd->write_motor(-123.4);
      counterPV = -99999;
      break;
    default:
      dd->write_pv((float)counterPV / 10);
      dd->write_motor((float)counterPV / -10);
      dd->write_bat((float)counterPV / 1000);
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
