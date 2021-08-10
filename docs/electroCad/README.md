# ELT-Schema and Board Design

[TOC]

## Open Questions

- [ ] Do we need 5V and 3.3V on Main board?
- [ ] Should we provide additional 5V to Steering-wheel board (brightness)?
- [ ] Do we need one or two CAN bus transceivers?
- [ ] The new designed elements (PCF, ADS) and other parts need to add a footprint in library

## Rules and Hints

- all new for SER4 designed elements should get the tag "SER4"

## Main-board

### Layout Idea
 - Board 150x100
![image-20210810101641377](.README/image-20210810101641377.png)

 --> The IO and ADC board are stripped of of all connectors and reassembled, see below)

![image-20210810103350670](.README/image-20210810103350670.png)

To change connectors:

- PCF8574 boards 
  - remove all plugs
  - reassembly 9 pin plug (8 IOs and Interrupt) and 4 pin I2C pin (I2C) on backside
  - can possibly be shortened a little
  - drill mounting hole
- ADS1115 grove boards
  - remove all plugs and connectors
  - reassembly 4 pins (analog in) and 4 pins (I2C) on backside
- CAN
  - IC names missing in doc/data-sheets

## Steering-wheel Board

