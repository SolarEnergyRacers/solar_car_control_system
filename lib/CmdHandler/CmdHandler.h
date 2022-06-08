//
// Command Receiver and Handler
//

#ifndef SOLAR_CAR_CONTROL_SYSTEM_CMDHANDLER_H
#define SOLAR_CAR_CONTROL_SYSTEM_CMDHANDLER_H

#include <abstract_task.h>

class CmdHandler : public abstract_task {
private:
  uint32_t sleep_polling_ms;

public:
  void set_SleepTime(uint32_t milliseconds) { sleep_polling_ms = milliseconds; };
  uint32_t get_SleepTime() { return sleep_polling_ms; }
  // RTOS task
  string getName(void) { return "CmdHandler"; };
  string init(void);
  string re_init(void);
  void exit(void);
  void task(void);

  // Class functions and members
  string commands = "!-:<>CcDdEHiIJklLMPRsSUuVvw";
  string helpText = "Available commands (" + commands +
                    "):\n"
                    "\t-------- SYSTEM COMMANDS -----------\n"
                    "\tR          - reset and reinit driver display\n"
                    "\tC          - switch to console screen\n"
                    "\tE          - switch to engineer screen\n"
                    "\tD          - switch to driver display\n"
                    "\t-          - adjust paddels\n"
                    "\tP          - print directory of sdcard\n"
                    "\ts          - print status of all values\n"
                    "\tS          - print status\n"
                    "\tJ          - write JSON status to sdcard\n"
                    "\t[V|v]      - write CSV status header (v - w/o header) to sdcard with\n"
                    "\tM          - mount sdcard and eneable logging\n"
                    "\tU          - unmount sdcard and disable logging\n"
                    "\tH          - memory_info\n"
                    "\tk kp ki kd - update PID constants\n"
                    "\t-------- DRIVER_RUNNING INFO COMMANDS -----------\n"
                    "\t:<text>    - display driver info text\n"
                    "\t!<text>    - display driver warn text\n"
                    "\tu [off]    - speed up arrow (green)      [|off]\n"
                    "\td [off]    - speed down arrow (red)      [|off]\n"
                    "\t-------- SUPPORT COMMANDS ------------------\n"
                    "\t< [off]    - left indicator              [|off]\n"
                    "\t> [off]    - right indicator             [|off]\n"
                    "\tw [off]    - hazard warning lights       [|off]\n"
                    "\tl [off]    - position lights on/off      [|off]\n"
                    "\tL [off]    - beam light on/off           [|off]\n"
                    "\tc [c|s|p]  - constant speed|power mode   [|c|s|p]\n"
                    "\tI          - scan I2C devices \n"
                    "\ti          - ioExt.readAll() \n"
                    "\t\n";

  string printSystemValues(void);
};

#endif // SOLAR_CAR_CONTROL_SYSTEM_CMDHANDLER_H
