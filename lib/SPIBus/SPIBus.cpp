//
// SPI Bus
//
#include <definitions.h>

#include <Console.h>
#include <fmt/core.h>
#include <inttypes.h>
#include <iostream>
#include <stdio.h>
#include <string>

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h> // semaphore

#include <SPI.h>
#include <SPIBus.h>
extern Console console;
using namespace std;

string SPIBus::re_init() { return init(); }

string SPIBus::init(void) {
  bool hasError = false;
  console << "[  ] Init 'SPI bus' with: SPI_CLK=" << SPI_CLK << ", SPI_MOSI=" << SPI_MOSI << ", SPI_MISO=" << SPI_MISO << "...\n";

  mutex = xSemaphoreCreateMutex();
  spi = SPIClass(VSPI);
  // init mutex (it is acquired)
  spi.begin(SPI_CLK, SPI_MISO, SPI_MOSI);

  // // initialize SPI:
  // spi.begin(SPI_CLK, SPI_MISO, SPI_MOSI, SPI_CS_SDCARD);
  // spi.setDataMode(SPI_MODE0);           // configuration of SPI communication in mode
  // spi.setClockDivider(SPI_CLOCK_DIV16); // configuration of clock at 1MHz
  // spi.end();

  xSemaphoreGive(mutex);
  // // CRITICAL SECTION SPI: end

  return fmt::format("[{}] SPIBus initialized.", hasError ? "--" : "ok");
}
