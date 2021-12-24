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

#include <GPIO.h>
#include <driver/gpio.h>

void GPInputOutput::re_init() { init(); }

void GPInputOutput::init() {
  // TODO
}

void GPInputOutput::exit() {
  // TODO
}

void GPInputOutput::register_gpio_interrupt() {

  // report
  cout << "[HW Interrupt] Register GPInputOutput interrupt pin " << GPIO_INTERRUPT_PIN << " (falling edge)" << endl;

  // set operating mode of interrupt pin to pull-up (i.e. interrupt is generated
  // if pin is getting grounded)
  pinMode(GPIO_INTERRUPT_PIN, INPUT_PULLUP);

  // register interrupt routine
  attachInterrupt(digitalPinToInterrupt(GPIO_INTERRUPT_PIN), handle_gpio_interrupt, FALLING);
}

volatile int GPInputOutput::interrupt_counter = 0;
portMUX_TYPE GPInputOutput::mux = portMUX_INITIALIZER_UNLOCKED;

void GPInputOutput::task() {

  // polling loop
  while (1) {

    // print the number of interrupts handled by the interrupt handler
    // handle_gpio_interrupt
    if (interrupt_counter > 0) {
      cout << "[INT] Number of interrupts: " << interrupt_counter << endl;
      interrupt_counter = 0;
    }

    // sleep for 1s
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}