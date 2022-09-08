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
  string commands = "!-:<>abBCcDEFGHiIJklLMPRsStTUVvw";
  string helpText = "Available commands (" + commands +
                    "):\n"
                    "\t-------- SYSTEM COMMANDS -------------------------\n"
                    "\tR _ _ _ _ _ _ _ _ _ _ reset and reinit driver display\n"
                    "\tC _ _ _ _ _ _ _ _ _ _ switch to console screen\n"
                    "\tE _ _ _ _ _ _ _ _ _ _ switch to engineer screen\n"
                    "\tD _ _ _ _ _ _ _ _ _ _ switch to driver display\n"
                    "\tP _ _ _ _ _ _ _ _ _ _ print directory of sdcard\n"
                    "\ts _ _ _ _ _ _ _ _ _ _ print status of all values\n"
                    "\tS _ _ _ _ _ _ _ _ _ _ print status\n"
                    "\tJ _ _ _ _ _ _ _ _ _ _ write JSON status to sdcard\n"
                    "\tV|v _ _ _ _ _ _ _ _ _ write CSV status to sdcard (V-with, v-w/o header)\n"
                    "\tM _ _ _ _ _ _ _ _ _ _ mount sdcard and eneable logging\n"
                    "\tU _ _ _ _ _ _ _ _ _ _ unmount sdcard and disable logging\n"
                    "\tH _ _ _ _ _ _ _ _ _ _ memory_info\n"
                    "\tB [rate]  _ _ _ _ _ _ show or set baud rate of Serial2\n"
                    "\tI _ _ _ _ _ _ _ _ _ _ scan I2C devices\n"
                    "\ti _ _ _ _ _ _ _ _ _ _ read IOs\n"
                    "\t   [i|o]               - show in|out continuously\n"
                    "\t   [a|d]               - show ADC|DAC continuously\n"
                    "\t   [c]                 - show controls acc/dec continuously\n"
                    "\t   [R]                 - reinit\n"
                    "\tb _ _ _ _ _ _ _ _ __  show CAN in\n"
                    "\tT [yyyy mm dd hh MM]  get/set RTC date and time\n"
                    "\tk [|kp ki kd|v] _ _ _ show / update PID constants, v- verbose display\n"
                    "\tG [-] _ _ _ _ _ _ _ _ Green light on  (-:off)\n"
                    "\tF [-] _ _ _ _ _ _ _ _ Fan on  (-:off)\n"
                    "\t-------- DRIVER INFO COMMANDS --------------------\n"
                    "\ta [u|d|o] _ _ _ _ _ _ speed arrow up | down | off\n"
                    "\t:<text> _ _ _ _ _ _ _ display driver info text\n"
                    "\t!<text> _ _ _ _ _ _ _ display driver warn text\n"
                    "\tu [-] _ _ _ _ _ _ _ _ green speed up arrow (-:off)\n"
                    "\td [-] _ _ _ _ _ _ _ _ red speed down arrow (-:off)\n"
                    "\t-------- Driver SUPPORT COMMANDS -----------------\n"
                    "\t- _ _ _ _ _ _ _ _ _ _ adjust paddels\n"
                    "\t< _ _ _ _ _ _ _ _ _ _ left indicator on/off\n"
                    "\t> _ _ _ _ _ _ _ _ _ _ right indicator on/off\n"
                    "\tw _ _ _ _ _ _ _ _ _ _ hazard warning lights on (-:off)\n"
                    "\tl [-] _ _ _ _ _ _ _ _ position lights on (-:off)\n"
                    "\tL [-] _ _ _ _ _ _ _ _ beam light on (-:off)\n"
                    "\tc [-|s|p] _ _ _ _ _ _ constant mode on (-:off|s:speed|p:power)\n";

  string printSystemValues(void);
};

#endif // SOLAR_CAR_CONTROL_SYSTEM_CMDHANDLER_H
