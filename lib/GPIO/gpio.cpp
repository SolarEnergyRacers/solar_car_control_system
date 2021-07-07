//
// General Purpose Input / Output
//

#include <definitions.h>

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <driver/gpio.h>

#include "gpio.h"

void blink_demo_task(void *pvParameter) {

  // configure the IOMUX register for pad LED_BUILTIN
  gpio_pad_select_gpio(LED_BUILTIN);

  // set the GPIO as a push/pull output
  gpio_set_direction(LED_BUILTIN, GPIO_MODE_OUTPUT);

  // polling loop
  while (1) {

    // blink off (output low)
    gpio_set_level(LED_BUILTIN, 0);

    // sleep for 1s
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    // blink on (output high)
    gpio_set_level(LED_BUILTIN, 1);

    // sleep for 1s
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

// add local variables required for the handler
portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
volatile int interrupt_counter = 0;
void register_gpio_interrupt() {

  // report
  printf("[HW Interrupt] Register gpio interrupt pin %d (falling edge)\n", GPIO_INTERRUPT_PIN);

  // set operating mode of interrupt pin to pull-up (i.e. interrupt is generated
  // if pin is getting grounded)
  pinMode(GPIO_INTERRUPT_PIN, INPUT_PULLUP);

  // register interrupt routine
  attachInterrupt(digitalPinToInterrupt(GPIO_INTERRUPT_PIN), handle_gpio_interrupt, FALLING);
}

void IRAM_ATTR handle_gpio_interrupt() {
  portENTER_CRITICAL_ISR(&mux);
  // count number of interrupts
  interrupt_counter++;
  portEXIT_CRITICAL_ISR(&mux);
}

void int_report_demo_task(void *pvParameter) {

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