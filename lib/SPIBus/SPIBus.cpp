//
// SPI Bus
//

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h> // semaphore

#include <SPI.h>

#include "../../include/definitions.h"

#include "SPIBus.h"

SemaphoreHandle_t spi_mutex;

void init_spi(void) {

//   // init mutex (it is acquired)
   spi_mutex = xSemaphoreCreateBinary();

//   // CRITICAL SECTION SPI: start
//   xSemaphoreTake(spi_mutex, portMAX_DELAY);

//     // initialize SPI:
//   SPI.begin(SPI_CLK, SPI_MOSI, SPI_MISO, SPI_CS_POTI);
//   SPI.begin(SPI_CLK, SPI_MOSI, SPI_MISO);
//   SPI.setDataMode(SPI_MODE0); // configuration of SPI communication in mode 0
//   SPI.setClockDivider(SPI_CLOCK_DIV16); // configuration of clock at 1MHz

   xSemaphoreGive(spi_mutex);
   // CRITICAL SECTION SPI: end

  printf("[v] SPI inited: SPI_CLK=%d, SPI_MOSI=%d, SPI_MISO=%d.\n",SPI_CLK, SPI_MOSI, SPI_MISO);
}
