# Changelog solar_car_control_system

## Hardware Changes

### DSUB Plug Main Board to Steering Wheel: SD_CS

Kabel Pin | Platine / Device
---|---
MainBoard DB12| ESP32:14 
SteeringWheel DB12 | ILI9341:SPI_CS_SDCARD 

### GPIO Pin 36 is not usable 

|                    | Old      | New      |
| ------------------ | -------- | -------- |
| ILI9341:SPI_CS_TFT | ESP32:36 | ESP32:32 |
|                    |          |          |

### SteeringWheel Board


Old|New
--|--|--
SD Card detect |% | IOExt2:5  
ILI C/D Low active	|R to GND	| R to VCC 
	|	|  

### Main Board

Old|New
--|--|--
 xbee | GND, VCC, TX, RX on Plug | GND,VCC,Tx, RX, SET on Plug 
