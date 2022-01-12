# Stand Elektrik / Elektronik / Software / Batterie

Date: 09.01.2022

## Elektrik

### Batterie

- Batterie aufgebaut
  - [x] Batteriekiste (teilweise)
  - [x] Zellenhalternung
  - [x] CMU-Halterung designed, gedruckt und eingebaut
  - [x] Zellen verschweisst
  - [x] geladen mit BMC Tests durchgeführt
  - [x] Layout für Batteryaufbau
  - [ ] Battery Montage Zellen + Elektrik
  - [ ] Testbattery

### Motoren

Ser1 und Ser2 Motor zur Effizienzprüfung nach Biel

- Entscheid über Verwendung des Antriebsstrangs offen:
  - [ ] Ser1
  - [x] Ser2 (Wunsch der Elektriker)

### BMS

- Getestet mit alter Ser2-Test Batterie
- CAN-Bus: 
  - [x] Verbingung PC ok.
  - [x] Verbindung BMC ok.
  - [x] Verbindung MPPT ok.
  - [ ] Integration in DataStorage und Kommunikation

### MPPT

- [x] geliefert
- CAN Bus:
  - [x] Verbindung zu BMC, PC, MPPT

### Elektronik

- [x] Prototyp 2 __Mainboard__ designed, geliefert, bestückt, vieles getestet
  - [x] Test und Verbesserungen
  - [ ] CAN Bus offen
- [x] Prototyp 2 __Schalterboard__ designed, geliefert, bestückt, getestet
  - [ ] Belegung zwei Reserverschalter noch offen
  - [x] 12V extra designed für V2
- [x] Prototyp 2 __Steeringwheel__ Board designed, geliefert, bestückt, vieles getestet
  - [x] Test und Verbesserungen (andere IO Extender, anderer ADC)
  - [ ] Modifikationen für SD Card getestet
  - [ ] Modifikationen für SD Card in ELT-Schema übernehmen

### Elektrik / Verkabelung

- Mainboard - Schalterboard: getestet, ok

- Mainboard - Steeringwheel Board: getestet, ok

- offen

  - [ ] Motorcontroller - Mainboard: diskrete Verkabelung
  - [ ] Temperatursensoren
  - [ ] Leuchten
  - [ ] Rückfahrkamera (getestet, Einabu noch offen)
  - [ ] Bremspedalschalter (designed, integriert in Mainboard, noch nicht getestet)
  - [ ] Funktübertragung Fahrdaten
    - [x] XBee ausgewählt
    - [ ] XBee Bestellung Frequenzband Australien
    - [ ] Test offen
    - [ ] Integration mit Mainboard als Console offen
  - [ ] Not-Aus-Schalter und deren mechanische Bedienung
  - [ ] PV Generator löten

### Software

siehe auch [github.com/SolarEnergyRacers/solar_car_control_system/projects/1](https://github.com/SolarEnergyRacers/solar_car_control_system/projects/1)

#### MC

- [x] Grundsystem aufgebaut (ESP32 mit FreeRTOS)

- [x] SPI, I2C Kommunikation
- [x] Driver Screen
- [x] Engineering Screen
- [x] CAN Bus
- [x] RTC
- [x] MicroSD
- [ ] Gyro
- [x] Relaisansteuerung (neue IO Extender)
- [x] ADC Test
- [x] DAC Test
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
