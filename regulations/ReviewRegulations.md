# Review Regulations World Solar Challenge (WSC), American Solar Challenge (ASC)

## ASC Regulaton [Release 2024 A](asc/ASC2024-Regs-EXTERNAL-RELEASE-A.pdf)
### 8.1 Sollar Collector (Jonas)
- [ ] Eine einzelne Solarzelle muss beim Scrutineering gezeigt werden.
- [ ] Mit einer Handpumpe darf Wasser auf die Solarzellen gesprüht werden, wenn das Solarauto stationär ist.

### 8.2 Energy Storage (Damian)
- [ ] Min. 3 Einzelzellen müssen bereitgestellt werden.
- [ ] 8.2.C.3: Bei einem Batteriefehler muss eine Hilfsbatterie das BMS, Strobe und Fehler Indication Licht betreiben.
### 8.3 Protection Circuits (Karl)

- [ ] Nachweis Batterieschultzschaltung
- [ ] Alle Schutzschaltungen im Batteriekasten
- [ ] max. 1mA Fehlerstrom
- [ ] **Automatischer** Batterienotschalter
  - [ ]  Batterienotschalter muss **manuell** und nicht fahrendem Fahrzeug wieder eingeschaltet werden
- [ ] 8.3.A.7 Battery Protection System (BPS): 
  - Monitoring und Auslösung: Überspannung, Übertemperatur, Überstrom, Unterspannung --> Zell-Level!
- Bei Laden von Hilfsbatterien im Auto, müssen diese auch überwacht und geschützt sein


### 8.4 Battery Enclosures (Damian)
- [x] Nicht Leitfähig
- [ ] Elektrolyt-Resistent 
- [ ] Chassis Isolation von 1MOhm
- [ ] Kleber mit “Caution: Chemical Hazard” and “High Voltage” und Batterietyp min 10mm hoch.
- [ ] ventilation systems must pull exhaust to the exterior of the solar car (Abzug-Prinzip?)

### 8.5 Main Fuse (Jonas)
- [ ] Eine DC-Sicherung (hight speed, kein Leistungsschalter) muss direkt an den positiven Pol der Batterie verbunden sein. -> Also Schmelzsicherung?
- [ ] All other wiring branching off the main bus circuit must have properly sized fuses. 
- [ ] Alle Messverbindungen vom BMS zu den Batteriezellen müssen abgesichert oder auf 1mA begrenzt werden. 

### 8.6 Power Switch (Jonas)
- [ ] Der Fahrer muss den Power Switch jederzeit ein- und ausschalten können.
- [ ] Der Powerswitch  muss mit den Worten "POWER", "ON" und "OFF" gekennzeichnet werden welche mindestens 10 mm hoch sind. Er muss vom Fahrer einfach erreichbar sein.
- [ ] In der OFF-Position muss die Batterie, der Motor und die Solarzellen voneinander getrennt sein. 
- [ ] Der Fahrer muss eine leuchtende Anzeige von einem allfälligen BMS-Fehler haben.
- [ ] Der externe Power Switch (aussen am Solarauto) muss auf der Fahrerseite sein und auf einer nach oben zeigenden Oberfläche angebracht werden. 
- [ ] Der Externe Powerswitch darf nur durch **Drücken** betätigt werden können und muss rot sein und einen Durchmesser von 20 mm haben.
- [ ] Die Betätigung muss ebenfalls die leuchtende Anzeige des BMS-Fehler aktivieren sowie den BMS Fault Dash Indicator (was ist das?).

### 8.7 Cables (Jonas)

### 8.8 Control (Karl)
- [ ] 8.8.A Nur Fahrer darf essentielle Fahrparameter einstellen können
- [x] Fahrpedal muss selbstständig zurückstellen
- [x] Cruise Control deaktiviert bei Bremsbetätigung
- [ ] Cruise Control deaktiviert bei Neustart 

### 8.9 Electrical Shock Hazard (Andi)
- exposed conductors >32V must be protected from human contact and marked `High Voltage` in >=10mm letters
### 9.4 Lighting (Tobias)
### 9.5 E Rear Vision (Damian)
- [ ] 60° FOV
- [ ] 200mm-dicker Pfeil muss in 15m entfernung erkennt werden.
### 10.5 Breakes (Karl)
- [ ] Zweikreisbremssystem, nicht links/rechts asymetrisch (Recuperation) zählt nicht
- [ ] Bremsflächje je Kontaktbereich: >6cm2
- [ ] Bremsbelagdicke bei Rennbeginn >6mm
- [ ] a-average >=4.72 m/s2 
  - [ ]  nur mechanisch (geht bei uns nicht)
  - [ ] Bei Hinterradbremsung: 10.5.G.2 Performance
 - [ ] Handbremse: 20% incline or decline

## WSC Regulation [Release 2021 B](wsc/2021_bwsc_regulations_release_version_1_19062020.pdf)
### 2.4 Solar colector (Jonas)
- 
### 2.18 Rear vision (Damian)
- [ ] Siehe Diagramm
- [x] Kamerasystem ist ok.
- [x] Nicht Seitenverkehrt
### 2.21 Brakes (Karl)
- [ ] Bremssystem zertifiziert
- [ ] Zweikreisbremssystem, nicht links/recht
  - [ ] Bremsweg Primärsystem : 0.1 v + 0.0060 v^2, in m from any speed v, in km/h
  - [ ] Bremsweg Primärsystem : 0.1 v + 0.0158 v^2, in m from any speed v, in km/h
- [ ] Handbremse: 10.6.A Performance

### 2.24 Lighting (Tobias)
### 2.25 Audible warning device (Tobias)
### 2.26 (Tobias, Karl)
- [x] the speed of the solar car
- [x] whether the direction indicators are operating
- [x] whether the hazard lights are operating
- [x] energy storage system warnings
- [ ] electronic rear vision images (if fitted)

### 2.27 Automatic functions (Karl)
- [x] Cruise Control deaktiviert bei Bremsbetätigung
- [ ] Cruise Control deaktiviert bei Neustart 

### 2.28/2.29 Electrical saftey, safe state (Jonas)
- [ ] Zum Schutz vor Hiht-voltage müssen doppelte Isolation oder Abdeckungen beutzt werden. Diese dprfen nur durch Tool entfernt werden können.
- [ ] Beim Fahrer muss zum Schutz vor High-voltage alle Abdeckungen maximal einen Spalt von 1mm aufweisen. Ansonsten muss dieser Spalt so klein sein, dass keine Finger dazwischen passen.
- [ ] The resistance between any exposed conductive part and each terminal of the energy storage system must exceed 100 V ohms, where V is the nominal voltage of the energy storage system.
- [ ]  A system must be implemented to monitor the isolation of the solar car’s chassis and body as per Regulations 2.28.7 and 2.28.8. This system may take the form of a permanently connected electronic device or a properly documented procedure of regular manual checks. 


### 3.8 Communication (Tobias)
### 3.13 Dynamic scrutineering (alle)

## ASC2016_Protection-System-Test-Procedure.pdf (Damian, Jonas)
- [ ] Sämtliche Schutzfunktionen des BMS werden getestet, man muss Messpunkte für Fehler-Signal-Injektion bereitstellen.

## iESC 2022

This regulatory document contains concrete dates and times (chapter scrutineering)

### 2.4 Solar Collector (Jonas)

### 2.5 Energy storage

### 2.6 Signage

### 2.8 Timing Transponder

### 2.15 Emergency recovery

### 2.24 Lighting

### 2.25 Audible warning device

### 2.26 Instrumentation (Karl)

### 2.27 Automatic functions (Karl)

### 2.28 Electrical safety

### 2.29 Electrical safe state 

###  2.30 Auxiliary Battery

###  3.8 Safety equipment

- [ ] battery datasheet
- [ ] suitable container for damaged electrochemial cells
- [ ] Sand

### 3.9 Communication

### 3.15 Charging with an External Power Supply

### 3.16 Solar Charging
