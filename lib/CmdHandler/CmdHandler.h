//
// Command Receiver and Handler
//

#ifndef SOLAR_CAR_CONTROL_SYSTEM_CMDHANDLER_H
#define SOLAR_CAR_CONTROL_SYSTEM_CMDHANDLER_H

#include <abstract_task.h>

class CmdHandler : public abstract_task {
public:
  // RTOS task
  string getName(void) { return "CmdHandler"; };
  string init(void);
  string re_init(void);
  void exit(void);
  void task(void);

  // Class functions and members
  string commands = "!-:<>CcDdEFGHiIJklLMPRsStTUuVvw";
  string helpText = "Available commands (" + commands +
                    "):\n"
                    "\t-------- SYSTEM COMMANDS -------------------------\n"
                    "\tR            - reset and reinit driver display\n"
                    "\tC            - switch to console screen\n"
                    "\tE            - switch to engineer screen\n"
                    "\tD            - switch to driver display\n"
                    "\t-            - adjust paddels\n"
                    "\tP            - print directory of sdcard\n"
                    "\ts            - print status of all values\n"
                    "\tS            - print status\n"
                    "\tJ            - write JSON status to sdcard\n"
                    "\tV|v          - write CSV status to sdcard (V-with, v-w/o header)\n"
                    "\tM            - mount sdcard and eneable logging\n"
                    "\tU            - unmount sdcard and disable logging\n"
                    "\tH            - memory_info\n"
                    "\tk [|kp ki kd|v] - show / update PID constants, v- verbose display\n"
                    "\tt            - get date and time\n"
                    "\tT yyyy mm dd hh MM - set RTC date and time\n"
                    "\tI            - scan I2C devices\n"
                    "\ti            - read IOs\n"
                    "\t   [i|o]        - show in|out continuously\n"
                    "\t   [a|d]        - show ADC|DAC continuously\n"
                    "\t   [c]          - show controls acc/dec continuously\n"
                    "\t   [R]          - reinit\n"
                    "\tG [-]        - Green light on  (-:off)\n"
                    "\tF [-]        - Fan on  (-:off)\n"
                    "\t-------- DRIVER INFO COMMANDS --------------------\n"
                    "\t:<text>      - display driver info text\n"
                    "\t!<text>      - display driver warn text\n"
                    "\tu [-]        - green speed up arrow (-:off)\n"
                    "\td [-]        - redspeed down arrow (-:off)\n"
                    "\t-------- Driver SUPPORT COMMANDS -----------------\n"
                    "\t<            - left indicator on/off\n"
                    "\t>            - right indicator on/off\n"
                    "\tw            - hazard warning lights on (-:off)\n"
                    "\tl [-]        - position lights on (-:off)\n"
                    "\tL [-]        - beam light on (-:off)\n"
                    "\tc [-|s|p]    - constant mode on (-:off|s:speed|p:power)\n"
                    "\t\n";

  string printSystemValues(void);
};

#endif // SOLAR_CAR_CONTROL_SYSTEM_CMDHANDLER_H
