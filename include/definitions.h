/*
 *
 */
#ifndef DEFINITIONS_H
#define DEFINITIONS_H


/*
 * ESP32 JTAG Debug Probe Wiring
 *  
 *  ESP32  - Probe
 *  --------------
 *  GPIO12 - TDI
 *  GPIO15 - TDO
 *  GPIO13 - TCK
 *  GPIO14 - TMS 
 * 
 *  Tutorial: https://medium.com/@manuel.bl/low-cost-esp32-in-circuit-debugging-dbbee39e508b
 *
 *  General ESP32 Pinout: https://randomnerdtutorials.com/esp32-pinout-reference-gpios/
 */


/*
 * ESP32 SPI
 *
 *  ESP32  - Probe
 *  --------------
 *  VSPI
 *  GPIO23   MOSI
 *  GPIO19   MISO
 *  GPIO18   CLK
 *  GPIO5    CS (first spi device)
 *
 *  HSPI
 *  GPIO13   MOSI
 *  GPIO12   MISO
 *  GPIO14   CLK
 *  GPIO15   CS
 *
 */
#define SPI_CS_SDCARD 5

#define LED_BUILTIN (gpio_num_t)22

#define I2C_SDA 33
#define I2C_SCL 32
#define I2C_FREQ 100000
SemaphoreHandle_t i2c_mutex;

#define ONEWIRE_PIN 2
SemaphoreHandle_t onewire_mutex;

#define GPIO_INTERRUPT_PIN 25

#endif // DEFINITIONS_H
