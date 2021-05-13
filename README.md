# <img src="media/logo/logo_green.svg" alt="SER Logo" width="150"/> SolarEnergyRacers

# Solar Car Control System ![PlatformIO CI - Build Process](https://github.com/SolarEnergyRacers/solar_car_control_system/workflows/PlatformIO%20CI%20-%20Build%20Process/badge.svg?branch=master) [![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

## ESP32 Pinout & Wiring

***Wiring***:
![ESP32 Pinout](media/Adafruit-HUZZAH32-Feather-pinout.png "ESP32 Pinout")

## Wiring / Port Usage

![Device Diagram](media/device_diagram.svg "Device Diagram")


| Device:Port | Functionality |
| :-          | :-:           |
| ***ESP32*** | main micro processor |
| ESP32:2  | 1Wire |
| ESP32:22 | I2C:SCL |
| ESP32:23 | I2C:SDA |
| ESP32:15 | SERIAL:TX |
| ESP32:16 | SERIAL:RX |
| ESP32:18 | SPI:MOSI |
| ESP32:19 | SPI:MISO |
| ESP32:5  | SPI:CLK |
| ESP32:36 | SPI:CS_TFT_SCREEN |
| ESP32:14 | SPI:CS_SD_CARD |
| ESP32:4  | SPI:DC |
| ESP32:21 | SPI:RST |
| ESP32:13 | GPIO:LED_BUILTIN |
| ESP32:25 | GPIO:INTERRUPT |
| ESP32:14 | GPIO:PCF8574_INTERRUPT |
| ESP32:25 | CAN:TX |
| ESP32:26 | CAN:RX |


| ***ADS1x15*** | Analog to digital converter |
| :- | :-: |
| ADC0, I2C Addr 0x48 |   |
| ADC0:0 | motor controller, speed indication |
| ADC0:1 | Ibat: battery current sensor |
| ADC0:2 | Imot: motor current sensor |
| ADC0:3 | Ipv: pv current sensor |
| ADC1, I2C Addr 0x49 (TODO: can be set??) |   |
| ADC1:0 | steering wheel acceleration piezo |
| ADC1:1 | steering wheel deceleration piezo |
| ADC1:2 |   |
| ADC1:3 |   |
| ADC2, I2C Addr 0x50 (TODO: can be set??) |   |
| ADC2:0 | Umot: motorcontroller voltage |
| ADC2:1 |   |
| ADC2:2 |   |
| ADC2:3 |   |


| ***PCF8574*** | Input/Output extender |
| :- | :-: |
| IOExt0, I2C Addr (TODO) |   |
| IOExt0:0 | switch: battery on/off |
| IOExt0:1 | switch: PV on/off |
| IOExt0:2 | switch: motor on/off |
| IOExt0:3 | mc: eco/power |
| IOExt0:4 | mc: on/off |
| IOExt0:5 | mc: fwd/rwd |
| IOExt0:6 | pv: on/off |
| IOExt0:7 | battery fan on/off |
| IOExt1, I2C Addr (TODO) |   |
| IOExt1:0 | output: left indicator |
| IOExt1:1 | output: right indicator |
| IOExt1:2 | output: horn |
| IOExt1:3 | output: head light |
| IOExt1:4 | output: light |
| IOExt1:5 |   |
| IOExt1:6 |   |
| IOExt1:7 |   |
| IOExt2, I2C Addr (TODO) | steering wheel buttons |
| IOExt2:0 | button: left indicator |
| IOExt2:1 | button: right indicator |
| IOExt2:2 | button: head light |
| IOExt2:3 | button: light |
| IOExt2:4 | button: horn |
| IOExt2:5 | button: next |
| IOExt2:6 | button: power/speed |
| IOExt2:7 | button: set/reset |


| ***ILI9341*** | 3.2" 320x240 display |
| :- | :-: |
| CS | see ESP32:SPI:CS_TFT_SCREEN |
| MOSI | see ESP32:SPI:MOSI |
| MISO | see ESP32:SPI:MISO (Pull Down: 10kΏ to GND) |
| CLK | see ESP32:SPI:CLK |
| D/S | Pull Down: 10kΏ to GND |


| ***DS1803***  | digital potentiometer |
| :- | :-: |
| SCL:8 | see ESP32:I2C:SCL |
| SDA:9 | see ESP32:I2C:SDA |
| H0:12 | VCC at 5V Powersupply |
| W0:10 | MC:acceleration input |
| L0:11 | GND at 5V Powersupply + ESP32:GND |
| H1:1 | VCC at 5V Powersupply |
| W1:3 | MC: deceleration input |
| L1:2 | GND at 5V Powersupply + ESP32:GND |
| A0:6 | GND for address 0x28 |
| A1:5 | GND for address 0x28 |
| A0:4 | GND for address 0x28 |


| ***RTC*** | real time clock |
| :- | :-: |
| SCL | see ESP32:I2C:SCL |
| SDA | see ESP32:I2C:SDA |


| ***Gyro&Acc*** | gyroscope & accelerator |
| :- | :-: |
| SCL | see ESP32:I2C:SCL |
| SDA | see ESP32:I2C:SDA |


| ***Temp*** | temperature sensor |
| :- | :-: |
| Temp0 | see ESP32:1Wire |
| Temp1 | see ESP32:1Wire |
| Temp2 | see ESP32:1Wire |
| Temp3 | see ESP32:1Wire |


| ***SD*** | sd card |
| :- | :-: |
| CS   | see ESP32:SPI:CS_SD_CARD |
| MOSI | see ESP32:MOSI |
| MISO | see ESP32:MISO |
| CLK  | see ESP32:CLK |
| D/S  | required? |


| ***Serial*** | serial telemetry communication |
| :- | :-: |
| RX | see ESP32:SERIAL:RX |
| TX | see ESP32:SERIAL:TX |


| ***MPPT*** | maximum power point tracking communication |
| :- | :-: |
| CAN Lo | see ESP32:CAN:Lo |
| CAN Hi | see ESP32:CAN:Hi |


| ***BMS*** | battery management system communication |
| :- | :-: |
| CAN Lo | see ESP32:CAN:Lo |
| CAN Hi | see ESP32:CAN:Hi |

## PlatformIO - Command Guide

***Build project***: `platformio run`

***Upload firmware***: `platformio run --target upload`

***Build specific environment***: `platformio run -e esp32dev`

***Upload firmware for the specific environment***: `platformio run -e esp32dev --target upload`

***Monitor device***: `platformio run device monitor`

***Clean build files***: `platformio run --target clean`

## License

This project is published under the [GNU General Public License v3.0](LICENSE).
