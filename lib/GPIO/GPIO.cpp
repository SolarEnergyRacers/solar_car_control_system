//
// General Purpose Input / Output
//

#include <definitions.h>

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <driver/gpio.h>

#include <GPIO.h>

void GPInputOutput::re_init() { init(); }

void GPInputOutput::init() {
  // TODO
}

void GPInputOutput::exit() {
  // TODO
}

void GPInputOutput::register_gpio_interrupt() {

  // report
  printf("[HW Interrupt] Register GPInputOutput interrupt pin %d (falling edge)\n", GPIO_INTERRUPT_PIN);

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
      printf("[INT] Number of interrupts: %d\n", interrupt_counter);
      interrupt_counter = 0;
    }

    // sleep for 1s
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}