# <img src="media/logo/logo_green.svg" alt="SER Logo" width="150"/> SolarEnergyRacers
# Solar Car Control System ![PlatformIO CI - Build Process](https://github.com/SolarEnergyRacers/solar_car_control_system/workflows/PlatformIO%20CI%20-%20Build%20Process/badge.svg?branch=master) [![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

## ESP32 Pinout & Wiring

***Wiring***:
![ESP32 Pinout](media/Adafruit-HUZZAH32-Feather-pinout.png "ESP32 Pinout")


## Wiring / Port Usage

![Device Diagram](media/device_diagram.svg "Device Diagram")


| Device:Port    | Functionality |
| :------------- | :-----------: |
| ***Analog to digital converter ADS1x15***  |  |
| ADC0, I2C Addr 0x49 |  |
| ADC0:0 | Motor Controller, speed indication |
| ADC0:1 |  |
| ADC0:2 |  |
| ADC0:3 |  |
| ADC1, I2C Addr TBD |  |
| ADC1:0 |  |
| ADC1:1 |  |
| ADC1:2 |  |
| ADC1:3 |  |
| ADC2, I2C Addr 0x48 |  |
| ADC2:0 | steering wheel acceleration potentiometer |
| ADC2:1 |  |
| ADC2:2 |  |
| ADC2:3 |  |
| ***Input/Output extender PCF8574***  |  |
| IOExt0, I2C Addr 0x38 |   |
| IOExt0:0 |  |
| IOExt0:1 |  |
| IOExt0:2 |  |
| IOExt0:3 |  |
| IOExt0:4 |  |
| IOExt0:5 |  |
| IOExt0:6 |  |
| IOExt0:7 |  |
| IOExt1, I2C Addr 0x20 | steering wheel buttons  |
| IOExt1:0 | left |
| IOExt1:1 | light |
| IOExt1:2 | LIGHT |
| IOExt1:3 |  |
| IOExt1:4 |  |
| IOExt1:5 |  |
| IOExt1:6 |  |
| IOExt1:7 | right |
| ***Display 3.2" 320x240 Adafruit ILI9341*** | driver display |
| CS  | |
| MOSI| |
| MISO| |
| CLK | |
| D/S | |

## PlatformIO - Command Guide

***Build project***: `platformio run`

***Upload firmware***: `platformio run --target upload`

***Build specific environment***: `platformio run -e esp32dev`

***Upload firmware for the specific environment***: `platformio run -e esp32dev --target upload`

***Monitor device***: `platformio run device monitor`

***Clean build files***: `platformio run --target clean`

## License

This project is published under the GNU General Public License v3.0, see [LICENSE](LICENSE).
