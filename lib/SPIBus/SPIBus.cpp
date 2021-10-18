//
// SPI Bus
//

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h> // semaphore

#include <SPI.h>

#include <definitions.h>

#include <SPIBus.h>

void SPIBus::re_init() { init(); }

void SPIBus::init(void) {
  // printf("    Init 'SPI bus' with: SPI_CLK=%d, SPI_MOSI=%d, SPI_MISO=%d.\n", SPI_CLK, SPI_MOSI, SPI_MISO);

  // init mutex (it is acquired)
  mutex = xSemaphoreCreateBinary();

  // // initialize SPI:
  // SPI.begin(SPI_CLK, SPI_MISO, SPI_MOSI);
  // SPI.setDataMode(SPI_MODE0);           // configuration of SPI communication in mode
  // SPI.setClockDivider(SPI_CLOCK_DIV16); // configuration of clock at 1MHz
  // SPI.end();

  xSemaphoreGive(mutex);
  // // CRITICAL SECTION SPI: end

  // printf("[v] SPI inited.\n");
}
