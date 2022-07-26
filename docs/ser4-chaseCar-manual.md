# SER4 Chase Car Manual

## Preparation

## Control Elements

### Mainboard

(nothing to do by the technicians)

### Emergency Controls

### Switch Board

### Steering Wheel

### Console Commands

Available commands (!-:<>CcDdEHiIJklLMPRsSUuVvw):

#### SYSTEM COMMANDS


| command          | Function                           |
| :----------------- | :----------------------------------- |
| R                | reset and reinit driver display    |
| C                | switch to console screen           |
| E                | switch to engineer screen          |
| D                | switch to driver display           |
| T yy mm dd hh MM | set rtc date and time              |
| t                | get date and time                  |
| -                | adjust paddels                     |
| P                | print directory of sdcard          |
| s                | print status of all values         |
| S                | print status                       |
| J                | write JSON status to sdcard        |
| v or V         | Log state as csv to sd card, V: with header (write csv data row into `/SER4DATA.CSV`) |
| M                | mount sdcard and eneable logging   |
| P                | print directory of sdcard          |
| U                | unmount sdcard and disable logging |
| ~H~              | ~memory_info~                      |
| k kp ki kd       | update PID constants               |
| I                | scan I2C devices                   |
| i                | IO ReadAll                         |
| ii               | verbose IO input view              |
| io               | verbose IO output view             |
|  |  |
| iR               | re-init IO                         |

#### DRIVER INFO COMMANDS


| command   | Function                        |
| ----------- | --------------------------------- |
| :`<text>` | display driver info text        |
| !`<text>` | display driver warn text        |
| u [-]     | green speed up arrow (- => off) |
| d [-]     | red speed down arrow (- => off) |

#### Driver SUPPORT COMMANDS


| command   | Function                     |
| ----------- | ------------------------------ |
| <         | left indicator on/off        |
| >         | right indicator on/off       |
| w         | hazard warning lights on/off |
| l [-]     | position lights on/off       |
| L [-]     | beam light on/off            |
| c [c/s/p] | constant speed/power mode    |

### Console Procedures

Mount SD card and start logging:

## Car Start

### Boot Screen

The bootscreen with blue font on yellow background shows the initialization of all devices controlled by the car control system.

The first part shows the device init, the second part shows the control task creation.

The last display is a 4 Seconds countdown to allow an additional view to the values.

### Adjust Paddles

After each start the Paddles must be adjusted. Until that the acceleration is locked. It is shown by the number `-99` in the `acceleration/deceleration` display and the drivers information Acceleration still locked.

Procedure:

- Send command `-`
- Move both paddles at same time slow from min to max to min.
  You have about 5s time for that.
- If successful the `acceleration/deceleration` display show `+ 0`
  Otherwise repeat the complete procedure

## Car Operate

## Steering Wheel

### Buttons

### Paddles

### Display
