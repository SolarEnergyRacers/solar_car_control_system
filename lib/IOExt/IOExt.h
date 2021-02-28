//
// PCF8574 I/O Extension over I2C  !!! UNTESTED !!!
//

#ifndef IOEXT_H
#define IOEXT_H

void init_ioext();
void set_ioext(int port, bool value);
int get_ioext(int port);
void key_pressed_interrupt_handler();
void io_ext_demo_task(void *pvParameter);

#endif // IOEXT_H
