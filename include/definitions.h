/*
 * Global definitions (pinout, device settings, ..)
 */
#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define ESP32Andy false
#define ESP32OLED false
#define ESP32Huzza true
#if ESP32Andy == true
/*
 *  GPIO
 */
#define LED_BUILTIN (gpio_num_t)22
#define GPIO_INTERRUPT_PIN 25
#define PCF8574_INTERRUPT_PIN 26

/*
 * OneWire
 */
#define ONEWIRE_PIN 2

/*
 * I2C
 */
#define I2C_SDA 33
#define I2C_SCL 32
#define I2C_FREQ 100000

#define I2C_ADDRESS_ADS1x15 0x48
#define I2C_ADDRESS_PCA9685 0x40
#define I2C_ADDRESS_SSD1305 0x3C
#define I2C_ADDRESS_PCF8574 0x38

// address = b0101{DS1803_ADDR2, DS1803_ADDR1, DS1803_ADDR0}
#define DS1803_BASE_ADDR 0x28
#define DS1803_ADDR0 0 // pulled down to ground
#define DS1803_ADDR1 0 // pulled down to ground
#define DS1803_ADDR2 0 // pulled down to ground
#define I2C_ADDRESS_DS1803 (DS1803_BASE_ADDR | (DS1803_ADDR2 << 2) | (DS1803_ADDR1 << 1) | DS1803_ADDR0)

/*
 *  SPI
 *
 *  ESP32  - SPI PIN
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
#define SPI_MOSI 18
#define SPI_MISO 19
#define SPI_CLK 18
#define SPI_CS_SDCARD 5

#define SPI_DC 12
#define SPI_CS_TFT 5
#define SPI_RST 17
#endif

#if ESP32OLED
#define SPI_MOSI 23
#define SPI_MISO 19
#define SPI_CLK 18
#define SPI_CS_SDCARD 5

#define SPI_DC 12
#define SPI_CS_TFT 5
#define SPI_RST 17
#endif

#if ESP32Huzza == true

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
//#define I2C_FREQ 400000 //400KHz

#define I2C_ADDRESS_ADS1x15 0x48
#define I2C_ADDRESS_PCA9685 0x42
#define I2C_ADDRESS_SSD1305 0x3C
#define I2C_ADDRESS_PCF8574 0x20

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
#define SPI_CS_SDCARD 14
#endif

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
