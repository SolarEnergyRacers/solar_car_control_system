# Checklist for SER-4 Electronik

## Hardware
### Main Board
### Switch Board
### Display Board
### BMC
### MPPTs
### Battery
### Back Camera

## Software
### Boot Screen



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

Simulate with CAN-Bus test program:

  - [ ] MPPT-1
  - [ ] MPPT-2
  - [ ] MPPT-3
  - [ ] Battery Msg

### Driver Screen

- [ ] Switch on from engineering display with driver display button

Simulate by switching on switch board:

  - [ ] MC on/off
  - [ ] Bat on/off
  - [ ] PV on/off

## Integration

### Indicators

- **Left**
  - [ ] left screen
  - [ ] left output *O_ind.Left*
  - Delay from button press to switch: ____ ms
- **Right**
  - [ ] right screen
  - [ ] right output *O_ind.Right*
  - Delay from button press to switch: ____ ms
- **Hazard Flash**
  - [ ] right+left screen
  - [ ] right output *O_ind.Right* + left output *O_ind.Left*
  - Delay from button press to switch: ____ ms

### Acceleration / Recuperation

- [ ] Adjust paddles by screen on boot screen

- [ ] Start adjust paddles by screen button P3

- [ ] Set tension left (press fully and release)

- [ ] Set tension right (press fully and release)

- [ ] Adjusted paddles values shown in driver display

- Press recuperation paddle
  - [ ] Negative screen values shown continuously vrom 0 to 99
  - [ ] *O_Decel* extends from 0 to 5 V
  - [ ] Press Break pedal give -88 in display  
  - [ ] Press Break pedal give 5V on *O_Decel*  
  
- Press acceleration paddle
  - [ ] Positive screen values shown continuously from 0 to 99
  - [ ] *O_Accel* extends from 0 to 5 V
  - [ ] Press Break pedal while acceleration paddle is pressed give -88 in display  
  - [ ] Press Break pedal while acceleration paddle is pressed give 5V on *O_Decel* 
  
  
