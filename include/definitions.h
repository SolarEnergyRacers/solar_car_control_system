/*
 * Global definitions (pinout, device settings, ..)
 */
#ifndef DEFINITIONS_H
#define DEFINITIONS_H

/*
 *  GPIO
 */
#define LED_BUILTIN (gpio_num_t)13
#define GPIO_INTERRUPT_PIN 25
#define PCF8574_INTERRUPT_PIN 14

/*
 * OneWire
 */
#define ONEWIRE_PIN 2

/*
 * I2C
 */
#define I2C_SDA 23
#define I2C_SCL 22
#define I2C_FREQ 100000 //100kHz

#define I2C_ADDRESS_ADS1x15 0x48
#define I2C_ADDRESS_PCA9685 0x42
#define I2C_ADDRESS_SSD1305 0x3C
#define I2C_ADDRESS_PCF8574 0x20

// address = b0101{DS1803_ADDR2, DS1803_ADDR1, DS1803_ADDR0}
#define DS1803_BASE_ADDR 0x28
#define DS1803_ADDR0 0 // pulled down to ground
#define DS1803_ADDR1 0 // pulled down to ground
#define DS1803_ADDR2 0 // pulled down to ground
#define I2C_ADDRESS_DS1803 (DS1803_BASE_ADDR | (DS1803_ADDR2 << 2) | (DS1803_ADDR1 << 1) | DS1803_ADDR0)

/*
 * SERIAL
 */
#define SERIAL_RX 16
#define SERIAL_TX 15
#define SERIAL_BAUDRATE 115200

/*
 * CAN Bus
 * 
 *  GPIO25  TX
 *  GPIO26  RX
 */
#define CAN_TX (gpio_num_t)25
#define CAN_RX (gpio_num_t)26
#define CAN_SPEED CAN_SPEED_250KBPS
#define CAN_RX_QUEUE 10


/*
 *  SPI
 *
 *  ESP32  - SPI PIN
 *  --------------
 *  VSPI
 *  GPIO19   MOSI
 *  GPIO18   MISO
 *  GPIO5    CLK
 *  GPIO21    CS (first spi device)
 *
 */
#define SPI_MOSI 18
#define SPI_MISO 19
#define SPI_CLK 5

#define SPI_DC 4
#define SPI_RST 21

#define SPI_CS_TFT 36
#define SPI_CS_POTI 33
#define SPI_CS_SDCARD 14

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

#endif // DEFINITIONS_H
