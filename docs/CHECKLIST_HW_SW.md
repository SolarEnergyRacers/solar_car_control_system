# Checklist for SER-4 Electronik

## Intro

The control elements described in the drivers manual (`ser4-drivers-manual.md`) are not explained wit images in this document. Please look there.

## Hardware

### Main Board

Main control unit for controlling the car electrically systems.

Includes the radio communication module for data transfer between the chase car and SER4 racing car.

### Switch Board

Additional board in the drivers cockpit with mechanical switches for car functions.

### Display Board

Integrated in the steering wheel. Shows all car data for engineers and driver. Displays short messages from the chase car to car driver.

### BMC

Battery management system. It is integrated in the battery box.

### Motor

Producer:

Power:   kW

### MPPTs

### Photo-voltaic Generator (PV)

Material:

Size:  m^2^

### Battery

Cell type:

Cell number:

Voltage working range:

### Back Camera

### Lights

#### Green Light

- show the switched off electric power system (PV, battery, motor)

#### Position Lights

- front (white)
- rear (red)
- side (?)

#### Indicators / Hazard Warning Lights

#### Driving Light

White front light

#### Break Lights

Switched by:

- Breakpedal buttons
- Recuperation paddle
- Constant Speed/Power mode car controller

### Break buttons

### Temperature Sensors



## Software

### Boot Screen

~~Boot screen waiting for `paddle adjust`~~ (Will be done manually by driver)

![phase1](assets.CHECKLIST_HW_SW/bootscreen-phase1.jpg)

Boot screen waiting for visual check (about 5s), then starts driver display

![phase2](assets.CHECKLIST_HW_SW/bootscreen-phase2.jpg)

### Engineering Screen

- [ ] Switch on from driver display with driver display button

Simulate by switching on switch board:

- [ ] MC on/off
- [ ] Bat on/off
- [ ] PV on/off

Simulate by turning potis on test console:

- [ ] BMS  I (*I_MC Voltage*) ???
- [ ] BMS U (*I_BAT Voltage*) ???
- [ ] U-min
- [ ] U-avg
- [ ] U-max
- [ ] T-1
- [ ] T-2
- [ ] T-3
- [ ] T-4

Simulate with CAN-Bus test program (TODO):

- [ ] MPPT-1
- [ ] MPPT-2
- [ ] MPPT-3
- [ ] Battery Msg

### Driver Screen

- [ ] Switch between engineering and with driver display (button next screen)

Simulate by switching on switch board:

- [ ] MC on/off (LED on connector board)
- [ ] Bat on/off (LED on connector board)
- [ ] PV on/off (LED on connector board)
- [ ] Speed

### SD Card

- [ ] Check mount - write log - unmount cycle

| Command (case sensitive) | Function                                                     |
| ------------------------ | ------------------------------------------------------------ |
| M                        | mount cd card                                                |
| P                        | print files and folders to console                           |
| V and v,v,v              | Log state as csv with header and then 3 data rows  to sd card (`/SER4DATA.CSV`) |
| J                        | write json data set into `/SER4DATA.CSV`                     |
| U                        | unmount sd card                                              |

- [ ] Life sign blinking red: no logging after Unmount or without inserted card
- [ ] Life sign blinking green: logging active and writing to sd card
- [ ] Inserting sd card mounts the card and switches on the logging automatically
- [ ] Check file and folder print
  - [ ] size increasing by log interval

## Integration

### Indicators

- **Left**

  - [ ] left screen
  - [ ] left output *O_ind.Left*  (LED on connector board)

  - Delay from button press to switch: ____ ms
- **Right**

  - [ ] right screen
  - [ ] right output *O\_ind.Right*  (LED on connector board)

  - Delay from button press to switch: ____ ms
- **Hazard Flash**

  - [ ] right+left screen
  - [ ] right output *O\_ind.Right* + left output *O_ind.Left*

  - Delay from button press to switch: ____ ms

### Acceleration / Recuperation

- [ ] Adjust paddles by screen on boot screen
- [ ] Acceleration display value `+ 0`
- [ ] (Re-)Start adjust paddles by screen button P3
- [ ] Timer runs on Driver Info Line
- [ ] Set tension left (press fully and release)
- [ ] Set tension right (press fully and release)

- Press acceleration and deceleration paddles simultaneously
  - [ ] give negative value in display
  - [ ] give >0 to 5V on *O_Decel*
- [ ] Adjusted paddles min/max values are shown in driver display
- [ ] Acceleration display value `+ 0`

- Press recuperation paddle

  - [ ] Negative screen values shown continuously vrom 0 to 99
  - [ ] *O_Decel* extends from 0 to 5 V
  - [ ] Press Break pedal
    - [ ] give -88 in display
    - [ ] give 5V on *O_Decel*
    - [ ] break light on

- Press acceleration paddle

  - [ ] Positive screen values shown continuously from 0 to 99
  - [ ] *O_Accel* extends from 0 to 5 V
  - Press Break pedal while acceleration paddle is pressed
    - [ ] give -88 in display
    - [ ] give 5V on *O_Decel*
    - [ ] break light on

- Set constant SPEED mode
  - [ ] dynamically give 1/- value in Accel display
  - [ ] dynamically give 0-5V on *O_Decel*
  - [ ] dynamically give 0-5V on *O_Accel*
