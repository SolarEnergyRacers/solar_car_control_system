# <img src="media/logo/logo_green.svg" alt="SER Logo" width="150"/> SolarEnergyRacers
# Solar Car Control System ![PlatformIO CI - Build Process](https://github.com/SolarEnergyRacers/solar_car_control_system/workflows/PlatformIO%20CI%20-%20Build%20Process/badge.svg?branch=master) [![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

## ESP32 Pinout & Wiring

***Wiring***:
![ESP32 Pinout](media/Adafruit-HUZZAH32-Feather-pinout.png "ESP32 Pinout")


## Wiring / Port Usage

![Device Diagram](media/device_diagram.svg "Device Diagram")


| Device:Port    | Functionality |
| :------------- | :-----------: |
| ***ESP32***  | main micro processor |
| ESP32:D12 | tbd |
| ESP32:D13 | tbd |
| ESP32:D14 | tbd |
| ESP32:D15 | tbd |
| ESP32:D16 | tbd |
| ESP32:D17 | tbd |
| ESP32:D18 | tbd |
| ESP32:D19 | tbd |
| ESP32:D21 | tbd |
| ESP32:D22 | tbd |
| ESP32:D23 | tbd |
| ESP32:D25 | tbd |
| ESP32:D26 | tbd |
| ESP32:D27 | tbd |
| ESP32:D32 | tbd |
| ESP32:D33 | tbd |
| ESP32:D34 | tbd |
| ESP32:D36 | tbd |
| ESP32:D39 | tbd |

| ***Analog to digital converter ADS1x15***  |  |
| :------------- | :-----------: |
| ADC0, I2C Addr 0x49 |  |
| ADC0:0 | motor controller, speed indication |
| ADC0:1 | Ibat: battery current sensor |
| ADC0:2 | Imot: motor current sensor |
| ADC0:3 | Ipv: pv current sensor  |
| ADC1, I2C Addr 0x48 |  |
| ADC1:0 | steering wheel acceleration piezo |
| ADC1:1 | steering wheel deceleration piezo |
| ADC1:2 | |
| ADC1:3 | |

| ***Input/Output extender PCF8574***  |  |
| :------------- | :-----------: |
| IOExt0, I2C Addr 0x38 |   |
| IOExt0:0 | switch: battery on/off  |
| IOExt0:1 | switch: PV on/off |
| IOExt0:2 | switch: motor on/off |
| IOExt0:3 | mc: eco/power |
| IOExt0:4 | mc: on/off |
| IOExt0:5 | mc: ?more? |
| IOExt0:6 | pv: on/off |
| IOExt0:7 | battery: ?more? |
| IOExt1, I2C Addr tbd |   |
| IOExt1:0 | |
| IOExt1:1 | |
| IOExt1:2 | |
| IOExt1:3 | |
| IOExt1:4 | |
| IOExt1:5 | |
| IOExt1:6 | |
| IOExt1:7 | |
| IOExt2, I2C Addr tbd | steering wheel buttons  |
| IOExt2:0 | left indicator |
| IOExt2:1 | right indicator |
| IOExt2:2 | head light |
| IOExt2:3 | light |
| IOExt2:4 | horn |
| IOExt2:5 | next |
| IOExt2:6 | power/speed |
| IOExt2:7 | set |

| ***Display 3.2" 320x240 Adafruit ILI9341*** | driver display (TODO: can't we use i2c?? to save #wires)|
| :------------- | :-----------: |
| CS   |  |
| MOSI |  |
| MISO |  |
| CLK  |  |
| D/S  |  |

| ***DigitPot*** | digital potentiometer |
| :------------- | :-----------: |
| SCL | tbd |
| SDA | tbd |

| ***RTC*** | real time clock |
| :------------- | :-----------: |
| SCL | tbd |
| SDA | tbd |

| ***Gyro&Acc*** | gyroscope & accelerator |
| :------------- | :-----------: |
| SCL  | tbd |
| SDA | tbd |

| ***Gyro&Acc*** | gyroscope & accelerator |
| :------------- | :-----------: |
| SCL  | tbd |
| SDA | tbd |

| ***Temp*** | temperature sensor |
| :------------- | :-----------: |
| Temp0  | addr: tbd |
| Temp1  | addr: tbd |
| Temp2  | addr: tbd|
| Temp3  | addr: tbd |

| ***SD*** | sd card |
| :------------- | :-----------: |
| CS   | tbd |
| MOSI | tbd |
| MISO | tbd |
| CLK  | tbd |
| D/S  | tbd |

| ***Serial*** | serial  telemetry communication |
| :------------- | :-----------: |
| RX  | tbd |
| TX  | tbd |

| ***MPPT*** | maximum power point tracking communication |
| :------------- | :-----------: |
| CAN Lo  | tbd |
| CAN Hi  | tbd |

| ***BMS*** | battery management system communication |
| :------------- | :-----------: |
| CAN Lo  | tbd |
| CAN Hi  | tbd |

## PlatformIO - Command Guide

***Build project***: `platformio run`

***Upload firmware***: `platformio run --target upload`

***Build specific environment***: `platformio run -e esp32dev`

***Upload firmware for the specific environment***: `platformio run -e esp32dev --target upload`

***Monitor device***: `platformio run device monitor`

***Clean build files***: `platformio run --target clean`

## License

This project is published under the GNU General Public License v3.0, see [LICENSE](LICENSE).
