//
// General Purpose Input / Output
//

#include <definitions.h>

#include <fmt/core.h>
#include <iostream>
#include <stdio.h>
#include <string>

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <Console.h>
#include <GPIO.h>
#include <driver/gpio.h>

extern Console console;

string GPIputOutput::re_init() { return init(); }

string GPIputOutput::init() {
  bool hasError = false;
  console << "[  ] Init 'GPIputOutput' ...";
  vPortCPUInitializeMutex(&mutex);
  // Init GPIO pins for CS of SD-card and TFT
  pinMode(SPI_CS_SDCARD, OUTPUT);
  digitalWrite(SPI_CS_SDCARD, HIGH);
  pinMode(SPI_CS_TFT, OUTPUT);
  digitalWrite(SPI_CS_TFT, HIGH);
  console << "done.\n";
  return fmt::format("[{}] SPI_CS for TFT and SD card set, GPIO initialized.", hasError ? "--" : "ok");
}

void GPIputOutput::exit() {
  // TODO
}

void GPIputOutput::register_gpio_interrupt() {

  // report
  console << "[HW Interrupt] Register GPIO interrupt pin " << GPIO_INTERRUPT_PIN << " (falling edge)\n";

  // set operating mode of interrupt pin to pull-up (i.e. interrupt is generated
  // if pin is getting grounded)
  pinMode(GPIO_INTERRUPT_PIN, INPUT_PULLUP);
  // register interrupt routine
  attachInterrupt(digitalPinToInterrupt(GPIO_INTERRUPT_PIN), handle_gpio_interrupt, FALLING);
}

volatile int GPIputOutput::interrupt_counter = 0;
portMUX_TYPE GPIputOutput::mutex = portMUX_INITIALIZER_UNLOCKED;

void GPIputOutput::task() {

  // polling loop
  while (1) {

    // print the number of interrupts handled by the interrupt handler
    // handle_gpio_interrupt
    if (interrupt_counter > 0) {
      console << "[INT] Number of interrupts: " << interrupt_counter << "\n";
      interrupt_counter = 0;
    }

    // sleep for 1s
    vTaskDelay(sleep_polling_ms / portTICK_PERIOD_MS);
  }
}