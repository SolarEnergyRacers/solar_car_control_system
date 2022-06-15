# Changelog solar_car_control_system

## Hardware Changes

### DSUB Plug Main Board to Steering Wheel: SD_CS

| Kabel Pin          | Platine / Device      |
| ------------------ | --------------------- |
| MainBoard DB12     | ESP32:14              |
| SteeringWheel DB12 | ILI9341:SPI_CS_SDCARD |

### GPIO Pin 36 is not usable

|                    | Old      | New      |
| ------------------ | -------- | -------- |
| ILI9341:SPI_CS_TFT | ESP32:36 | ESP32:32 |
|                    |          |          |

### SteeringWheel Board

|                    | Old      | New      |
| ------------------ | -------- | -------- |
| SD Card detect     | %        | IOExt2:5 |
| ILI C/D Low active | R to GND | R to VCC |
|                    |          |          |

### Main Board

| Old               | New                      |                                             |
| ----------------- | ------------------------ | ------------------------------------------- |
| Serial Tranceiver | GND, VCC, TX, RX on Plug | GND,VCC,Tx, RX, SET on Plug                 |
| LTC4311           | Integrate into mainboard | remark: near ESP32 SDA,SCL or near MCP23017 |

|                    | Old      | New      |
| ------------------ | -------- | -------- |
| SD Card detect     | %        | IOExt2:5 |
| ILI C/D Low active | R to GND | R to VCC |





## 2022.06.14 -  Test im Auto

- [ ] CAN auf 12V
- [ ] Mainboard 12V und GND am Inputstecker brücken
- [ ] PullDown nach Z-Diode
- [ ] Neues Konzept DAC-Abschaltung
- [ ] Sicherheit gehen Verpolung des 12V Abschalters
