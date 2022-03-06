/*
 * Global definitions (pinout, device settings, ..)
 */
#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define VERSION "v0.2"
#define VERSION_PUBLISHED "2022.02"

#include <LocalFunctionsAndDevices.h>

/*
 *  GPIO
 */
#define GPIO_INTERRUPT_PIN 33

/*
 * OneWire
 */
#define ONEWIRE_PIN 12

/*
 * I2C
 */
#define I2C_SDA 23
#define I2C_SCL 22
#define I2C_FREQ 200000 // 200kHz
//#define I2C_FREQ 100000 // 100kHz
//#define I2C_FREQ 50000 // 50kHz

#define NUM_ADC_DEVICES 3
#define I2C_ADDRESS_ADS1x15_0 0x48
#define I2C_ADDRESS_ADS1x15_1 0x49
#define I2C_ADDRESS_ADS1x15_2 0x4a
#define ADC_MAX 65535

#define PWM_NUM_PORTS 16
#define PWM_MAX_VALUE 4096
#define I2C_ADDRESS_PCA9685 0x42

#define MCP23017_NUM_DEVICES 2
#define MCP23017_NUM_PORTS 16
#define IOExtPINCOUNT (MCP23017_NUM_DEVICES * MCP23017_NUM_PORTS)
#define I2C_ADDRESS_MCP23017_IOExt0 0x20
#define I2C_ADDRESS_MCP23017_IOExt1 0x21
#define I2C_INTERRUPT 33

// address = b0101{DS1803_ADDR2, DS1803_ADDR1, DS1803_ADDR0}
#define DS1803_BASE_ADDR 0x28
#define DS1803_ADDR0 0 // pulled down to ground
#define DS1803_ADDR1 0 // pulled down to ground
#define DS1803_ADDR2 0 // pulled down to ground
#define I2C_ADDRESS_DS1803 (DS1803_BASE_ADDR | (DS1803_ADDR2 << 2) | (DS1803_ADDR1 << 1) | DS1803_ADDR0)

/*
 * SERIAL2
 */
#define SERIAL_BAUDRATE 115200
#define SERIAL2_RX 16
#define SERIAL2_TX 17
#define SERIAL2_BAUDRATE 9600

/*
 * CAN Bus
 *
 *  GPIO25  TX
 *  GPIO26  RX
 */
#define CAN_TX (gpio_num_t)26
#define CAN_RX (gpio_num_t)25
#define CAN_SPEED 250E3 // Change CAN Speed on BMS to 125
#define BMS_BASE_ADDR 0x700
#define MPPT1_BASE_ADDR 0x600
#define MPPT2_BASE_ADDR 0x610
#define MPPT3_BASE_ADDR 0x620
#define CAN_RX_BUFFER_SIZE 20

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

#define SPI_CS_TFT 32
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

/*
 * carState and carControl
 */
#define LOG_INTERVALL 1000;

#endif // DEFINITIONS_H
