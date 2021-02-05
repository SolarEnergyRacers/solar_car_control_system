//
// General Purpose Input / Output
//

#ifndef GPIO_H
#define GPIO_H

void blink_demo_task(void *pvParameter);

void int_report_demo_task(void *pvParameter);
void register_gpio_interrupt(void);
void IRAM_ATTR handle_gpio_interrupt(void);

#endif // GPIO_H
