//
// General Purpose Input / Output
//

#ifndef GPIO_H
#define GPIO_H

#include <definitions.h>
#include <abstract_task.h>
#include <driver/gpio.h>

class GPInputOutput : public abstract_task {
private:
    // add local variables required for the handler
public:
    string getName(void);
    void init(void);
    void re_init(void);
    void exit(void);
    void task(void);

    void register_gpio_interrupt(void);
    static volatile int interrupt_counter;
    static portMUX_TYPE mux;

    static void IRAM_ATTR handle_gpio_interrupt(void){
        portENTER_CRITICAL_ISR(&mux);
        // count number of interrupts
        interrupt_counter++;
        portEXIT_CRITICAL_ISR(&mux);
    };
};

#endif // GPIO_H
