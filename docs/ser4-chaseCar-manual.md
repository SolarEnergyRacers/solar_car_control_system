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

Available commands (!-:<>abBCcDEFGHiIJklLMPRsStTUVvw):
```
        -------- SYSTEM COMMANDS -------------------------
        R _ _ _ _ _ _ _ _ _ _ reset and reinit driver display
        C _ _ _ _ _ _ _ _ _ _ switch to console screen
        E _ _ _ _ _ _ _ _ _ _ switch to engineer screen
        D _ _ _ _ _ _ _ _ _ _ switch to driver display
        P _ _ _ _ _ _ _ _ _ _ print directory of sdcard
        s _ _ _ _ _ _ _ _ _ _ print status of all values
        S _ _ _ _ _ _ _ _ _ _ print status
        J _ _ _ _ _ _ _ _ _ _ write JSON status to sdcard
        V|v _ _ _ _ _ _ _ _ _ write CSV status to sdcard (V-with, v-w/o header)
        M _ _ _ _ _ _ _ _ _ _ mount sdcard and eneable logging
        U _ _ _ _ _ _ _ _ _ _ unmount sdcard and disable logging
        H _ _ _ _ _ _ _ _ _ _ memory_info
        B [rate]  _ _ _ _ _ _ show or set baud rate of Serial2
        I _ _ _ _ _ _ _ _ _ _ scan I2C devices
        i _ _ _ _ _ _ _ _ _ _ read IOs
           [i|o]               - show in|out continuously
           [a|d]               - show ADC|DAC continuously
           [c]                 - show controls acc/dec continuously
           [R]                 - reinit
        b _ _ _ _ _ _ _ _ __  show CAN in
        T [yyyy mm dd hh MM]  get/set RTC date and time
        k [|kp ki kd|v] _ _ _ show / update PID constants, v- verbose display
        G [-] _ _ _ _ _ _ _ _ Green light on  (-:off)
        F [-] _ _ _ _ _ _ _ _ Fan on  (-:off)
        -------- DRIVER INFO COMMANDS --------------------
        a [u|d|o] _ _ _ _ _ _ speed arrow up | down | off
        :`<text>` _ _ _ _ _ _ _ display driver info text
        !`<text>` _ _ _ _ _ _ _ display driver warn text
        u [-] _ _ _ _ _ _ _ _ green speed up arrow (-:off)
        d [-] _ _ _ _ _ _ _ _ red speed down arrow (-:off)
        -------- Driver SUPPORT COMMANDS -----------------
        - _ _ _ _ _ _ _ _ _ _ adjust paddels
        < _ _ _ _ _ _ _ _ _ _ left indicator on/off
        > _ _ _ _ _ _ _ _ _ _ right indicator on/off
        w _ _ _ _ _ _ _ _ _ _ hazard warning lights on (-:off)
        l [-] _ _ _ _ _ _ _ _ position lights on (-:off)
        L [-] _ _ _ _ _ _ _ _ beam light on (-:off)
        c [-|s|p] _ _ _ _ _ _ constant mode on (-:off|s:speed|p:power)
```

#### SYSTEM COMMANDS

| command          | Function                                                                                |
| :--------------- | :-------------------------------------------------------------------------------------- |
| R                | reset and reinit driver display                                                         |
| C                | switch to console screen                                                                |
| E                | switch to engineer screen                                                               |
| D                | switch to driver display                                                                |
| T yy mm dd hh MM | set rtc date and time                                                                   |
| t                | get date and time                                                                       |
| -                | adjust paddels                                                                          |
| P                | print directory of sdcard                                                               |
| s                | print status of all values                                                              |
| S                | print status                                                                            |
| J                | write JSON status to sdcard                                                             |
| v or V           | Log state as csv to sd card, V: with header (write csv data row into `/SER4DATA.CSV`) |
| M                | mount sdcard and eneable logging                                                        |
| P                | print directory of sdcard                                                               |
| U                | unmount sdcard and disable logging                                                      |
| ~H~             | ~memory_info~                                                                          |
| k kp ki kd       | update PID constants                                                                    |
| I                | scan I2C devices                                                                        |
| i                | IO ReadAll                                                                              |
| ii               | verbose IO input view                                                                   |
| io               | verbose IO output view                                                                  |
|                  |                                                                                         |
| iR               | re-init IO                                                                              |

#### DRIVER INFO COMMANDS

| command     | Function                        |
| ----------- | ------------------------------- |
| :`<text>` | display driver info text        |
| !`<text>` | display driver warn text        |
| u [-]       | green speed up arrow (- => off) |
| d [-]       | red speed down arrow (- => off) |

#### Driver SUPPORT COMMANDS

| command   | Function                     |
| --------- | ---------------------------- |
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
