//
// PCF8574 I/O Extension over I2C  !!! UNTESTED !!!
//

#ifndef IOEXT_H
#define IOEXT_H

// public
void set_ioext(int port, bool value);
int get_ioext(int port);

// internal
void keyPressedInterruptHandler();

// FreeRTOS - start
void init_IOExt2();
void IOExt2_task(void *pvParameter);
// FreeRTOS - end

#endif // IOEXT_H
