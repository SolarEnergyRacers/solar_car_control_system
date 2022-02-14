# Changelog solar_car_control_system

## Hardware Changes

### DSUB Stecker Main Board to Steering Wheel: SD_CS

Kabel Pin | Platine / Device
---|---
MainBoard DB12| ESP32:14 
SteeringWheel DB12 | ILI9341:SPI_CS_SDCARD 

### GPIO Pin 36 is not usable 

|                    | Old      | New      |
| ------------------ | -------- | -------- |
| ILI9341:SPI_CS_TFT | ESP32:36 | ESP32:32 |
|                    |          |          |
