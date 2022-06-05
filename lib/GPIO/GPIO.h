//
// General Purpose Input / Output
//

#ifndef SOLAR_CAR_CONTROL_SYSTEM_GPIO_H
#define SOLAR_CAR_CONTROL_SYSTEM_GPIO_H

#include <abstract_task.h>
#include <definitions.h>
#include <driver/gpio.h>

class GPIputOutput : public abstract_task { // Note: This class is called weirdly due to a naming conflict. Suggestions are welcome :)
private:
  // uint32_t sleep_polling_ms = 400;

public:
  // RTOS task
  string getName(void) { return "GPIO"; };
  string init(void);
  string re_init(void);
  void exit(void);
  void task(void);

  // Class functions and members
  void register_gpio_interrupt(void);
  static volatile int interrupt_counter;
  static portMUX_TYPE mux;

  static void IRAM_ATTR handle_gpio_interrupt(void) {
    portENTER_CRITICAL_ISR(&mux);
    // count number of interrupts
    interrupt_counter++;
    portEXIT_CRITICAL_ISR(&mux);
  };
};

#endif // SOLAR_CAR_CONTROL_SYSTEM_GPIO_H
