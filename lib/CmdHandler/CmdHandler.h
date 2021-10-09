//
// Command Receiver and Handler
//

#ifndef SOLAR_CAR_CONTROL_SYSTEM_CMDHANDLER_H
#define SOLAR_CAR_CONTROL_SYSTEM_CMDHANDLER_H

#include <abstract_task.h>

class CmdHandler : public abstract_task {
private:
  string commands = "RS-=:!udsaAbpm<>wlLc";
  string helpText = "Available commands (" + commands +
                    "):\n"
                    "\t-------- SYSTEM COMMANDS -----------\n"
                    "\tR        - reset and reinit display\n"
                    "\t-        - set min accel and recup value (paddels released)\n"
                    "\t=        - set max accel and recup value (paddels pressed)\n"
                    "\tS        - print status of all values\n"
                    "\t-------- DRIVER INFO COMMANDS -----------\n"
                    "\t:<text>  - display driver info text\n"
                    "\t!<text>  - display driver warn text\n"
                    "\tu [off]  - speed up arrow (green)    [|off]\n"
                    "\td [off]  - speed down arrow (red)    [|off]\n"
                    "\t-------- TEST COMMANDS ------------------\n"
                    "\ts ddd    - speed value               [0...999]\n"
                    "\ts [f|b]  - drive forward|backward    [f|b]\n"

                    "\ta dd     - acceleration value        [-9...+9]\n"
                    "\tA ddd    - set poti          recu:   [-255...0] accel: [0...255]\n"

                    "\tb fff.f  - battary voltage           [0...999]\n"
                    "\tp ffff.f - photovoltaics current     [-999...+999]\n"
                    "\tm ffff.f - motor current             [-999...+999]\n"
                    "\t< [off]  - left indicator            [ |off]\n"
                    "\t> [off]  - right indicator           [ |off]\n"
                    "\tw [off]  - hazard warning lights     [ |off]\n"
                    "\tl [off]  - position lights on/off    [ |off]\n"
                    "\tL [off]  - beam light on/off         [ |off]\n"
                    "\tc [s|p]  - constant speed|power mode [s|p]\n"
                    "\t\n";

  void printSystemValues(void);

public:
  void init();
  void re_init();
  string getName(void) { return "CmdHandler"; };
  void exit(void);
  void task(void); // this is the actual task
};

#endif // SOLAR_CAR_CONTROL_SYSTEM_CMDHANDLER_H
