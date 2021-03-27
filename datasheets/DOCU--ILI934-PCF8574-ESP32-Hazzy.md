# DOCU ILI9341 + PCF8574 + ESP32-Hazzy

## ILI9341 + PCF8574 + ADS1115

10 Kabel vom Miroprocessor zum Display/Buttons/ReglerLeistungsaufnahme Experimentalaufbau mit 

- Display ILI9341 (320x240 pixel)
- IO Extender PCF8574 (8 bit) (7 Taster und 1 LED)
- Analog-Digital-Wandler (12bit) und Potentiometer

![image-20210323171846165](DOCU--ILI934-PCF8574-ESP32-Hazzy.assets/2021-03-27 09.09.17.jpg)

## U-I-Diagramm bei Spannungsänderung

| Spannung<br />[V] | Strom<br />[mA] | Leistung<br />[mW] |
| ----------------- | --------------- | ------------------ |
| 3.0               | 20              | 60                 |
| 3.3               | 32              | 106                |
| 3.8               | 59              | 224                |
| 4.0               | 73              | 292                |
| 5.0               | 85              | 425                |

## U-I-Diagramm bei Displayänderung

Es wurde bei Vollbild Black und Vollbild White gemessen:

| Spannung [V] | Strom Black  [mA] | Strom White  [mA] | Leistung Black[mW] | Leistung White  [mW] |
| :----------: | :---------------: | :---------------: | :----------------: | :------------------: |
|     3.05     |        25         |        24         |         75         |          72          |
|     3.3      |        30         |        29         |         99         |          96          |
|     3.8      |        54         |        53         |        205         |         201          |
|     4.0      |        63         |        62         |        252         |         248          |
|     5.0      |        83         |        80         |        415         |         400          |

![image-20210327121949117](DOCU--ILI934-PCF8574-ESP32-Hazzy.assets/image-20210327121949117.png)