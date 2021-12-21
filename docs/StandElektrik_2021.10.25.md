# Stand Elektrik / Elektronik / Software / Batterie

## Elektrik

### Batterie

- Batterie aufgebaut
  - [ ] Batteriekiste (teilweise)
  - [x] Zellenhalternung
  - [ ] CMU-Halterung designed, muss gedruckt werden
  - [x] Zellen verschweisst
  - [x] geladen mit BMC Tests durchgeführt

### Motoren

Ser1 und Ser2 Motor zur Effizienzprüfung nach Biel

- Entscheid über Verwendung des Antriebsstrangs offen:
  - [ ] Ser1
  - [ ] Ser2 

### BMS

- Getestet mit alter Ser2-Test Batterie
- CAN-Bus: 
  - [x] Verbingung PC ok.
  - [ ] Verbindung MC noch in Exploration

### MPPT

- [x] geliefert
- CAN Bus:
  - [ ] Verbindung zu MC noch in Exploration

### Elektronik

- [x] Prototyp 1 __Mainboard__ designed, geliefert, bestückt, vieles getestet
  - [ ] Test und Verbesserungen (andere IO Extender, anderer ADC)
  - [ ] CAN Bus offen
- [x] Prototyp 1 __Schalterboard__ designed, geliefert, bestückt, getestet
  - [ ] Belegung zwei Reserverschalter noch offen
  - [x] 12V extra designed für V2
- [x] Prototyp 1 __Steeringwheel__ Board designed, geliefert, bestückt, vieles getestet
  - [ ] Test und Verbesserungen (andere IO Extender, anderer ADC)

### Elektrik / Verkabelung

- Mainboard - Schalterboard: getestet, ok

- Mainboard - Steeringwheel Board: getestet, ok

- offen

  - [ ] CAN Bus MPPTs - Mainboard
  - [ ] CAN Bus BMC - Mainboard
  - [ ] Motorcontroller - Mainboard: diskrete Verkabelung
  - [ ] Temperatursensoren
  - [ ] Leuchten
  - [ ] Rückfahrkamera (getestet, Einabu noch offen)
  - [ ] Bremspedalschalter (designed, noch nicht getestet)
  - [ ] Funktübertragung Fahrdaten
    - [x] ANT ausgewählt
    - [ ] Test offen
  - [ ] Not-Aus-Schalter und deren mechanische Bedienung
  - [ ] PV Generator löten

### Software

siehe auch [github.com/SolarEnergyRacers/solar_car_control_system/projects/1](https://github.com/SolarEnergyRacers/solar_car_control_system/projects/1)

#### MC

- [x] Grundsystem aufgebaut (ESP32 mit FreeRTOS)

- [x] SPI, I2C Kommunikation
- [x] Driver Screen
- [x] Engineering Screen
- offen:
  - [ ] CAN Bus
  - [x] RTC
  - [ ] MicroSD
  - [ ] Gyro
  - [ ] Relaisansteuerung (neue IO Extender)
  - [ ] ADC Test
  - [ ] DAC Test
  - [ ] Dokumentation

#### Chase Car Computer

- [x] Datenbank designed
- [x] Grundprogramm zu Datenaufbereitung
- offen:
  - [ ] Datenablage
  - [ ] Datenaufbereitung
  - [ ] Dokumentation

#### Service Computer

- Prohelion installiert
- CAN Konfiguration
- offen:
  - [ ] Dokumentation
