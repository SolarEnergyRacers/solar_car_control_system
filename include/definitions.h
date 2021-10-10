/*
 * Global definitions (pinout, device settings, ..)
 */
#ifndef DEFINITIONS_H
#define DEFINITIONS_H

/*
 *  GPIO
 */
//#define LED_BUILTIN (gpio_num_t)13
#define GPIO_INTERRUPT_PIN 33 // 25

/*
 * OneWire
 */
#define ONEWIRE_PIN 2

/*
 * I2C
 */
#define I2C_SDA 23
#define I2C_SCL 22
#define I2C_FREQ 100000 // 100kHz

#define NUM_ADC_DEVICES 3
#define I2C_ADDRESS_ADS1x15_0 0x4A
#define I2C_ADDRESS_ADS1x15_1 0x48
#define I2C_ADDRESS_ADS1x15_2 0x49

#define PWM_NUM_PORTS 16
#define PWM_MAX_VALUE 4096
#define I2C_ADDRESS_PCA9685 0x42

#define OLED_RESET 9
#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define I2C_ADDRESS_SSD1305 0x3C

#define PCF8574_NUM_DEVICES 4
#define PCF8574_NUM_PORTS 8
#define I2C_ADDRESS_PCF8574_IOExt0 0x20 // TODO: set jumpers accordingly
#define I2C_ADDRESS_PCF8574_IOExt1 0x21 // TODO: set jumpers accordingly
#define I2C_ADDRESS_PCF8574_IOExt2 0x22 // TODO: set jumpers accordingly
#define I2C_ADDRESS_PCF8574_IOExt3 0x23 // TODO: set jumpers accordingly
#define I2C_INTERRUPT_PIN_PCF8574 33
#define USE_LEGACY_PIN_READ

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
#define SERIAL_TX 17
#define SERIAL_BAUDRATE 115200

/*
 * CAN Bus
 *
 *  GPIO25  TX
 *  GPIO26  RX
 */
#define CAN_TX (gpio_num_t)26
#define CAN_RX (gpio_num_t)25
#define CAN_SPEED 250E3 //Change CAN Speed on BMS to 125
#define BMS_BASE_ADDR 0x700
#define MPPT0_BASE_ADDR 0x600
#define MPPT1_BASE_ADDR 0x610
#define MPPT2_BASE_ADDR 0x620
#define CAN_RX_BUFFER_SIZE 10

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
 *  Tutorial:
 * https://medium.com/@manuel.bl/low-cost-esp32-in-circuit-debugging-dbbee39e508b
 *
 *  General ESP32 Pinout:
 * https://randomnerdtutorials.com/esp32-pinout-reference-gpios/
 */

#define DEBUG true
#define debug_printf(fmt, ...)                                                                                                             \
  do {                                                                                                                                     \
    if (DEBUG)                                                                                                                             \
      fprintf(stderr, "%-32s:%3d %-36s: " fmt, __FILE__, __LINE__, __func__, __VA_ARGS__);                                                 \
  } while (0)

#define DEBUG2 false
#define debug_printf_l2(fmt, ...)                                                                                                          \
  do {                                                                                                                                     \
    if (DEBUG2)                                                                                                                            \
      fprintf(stderr, "%-32s:%3d %-36s: " fmt, __FILE__, __LINE__, __func__, __VA_ARGS__);                                                 \
  } while (0)

#endif // DEFINITIONS_H
