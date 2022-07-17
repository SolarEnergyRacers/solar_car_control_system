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
                    "\t[V|v]      - write CSV status (V - with, v - w/o header) to sdcard\n"
                    "\tM          - mount sdcard and eneable logging\n"
                    "\tU          - unmount sdcard and disable logging\n"
                    "\tH          - memory_info\n"
                    "\tk kp ki kd - update PID constants\n"
                    "\t-------- DRIVER_RUNNING INFO COMMANDS -----------\n"
                    "\t:<text>    - display driver info text\n"
                    "\t!<text>    - display driver warn text\n"
                    "\tu [-]      - speed up arrow (green)\n"
                    "\td [-]      - speed down arrow (red)\n"
                    "\t-------- SUPPORT COMMANDS ------------------\n"
                    "\t<          - left indicator on/off\n"
                    "\t>          - right indicator on/off\n"
                    "\tw          - hazard warning lights on/off\n"
                    "\tl [-]      - position lights on/off\n"
                    "\tL [-]      - beam light on/off\n"
                    "\tc [c|s|p]  - constant speed|power mode\n"
                    "\tI          - scan I2C devices\n"
                    "\ti [|o|R]   - ioExt.readAll (o - ioExt show in console continously, R -ioExt.re_init)\n"
                    "\t\n";

  string printSystemValues(void);
};

#endif // SOLAR_CAR_CONTROL_SYSTEM_CMDHANDLER_H
