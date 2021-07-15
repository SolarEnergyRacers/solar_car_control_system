//
// Command Receiver and Handler
//

#ifndef CMDHANDLER_H
#define CMDHANDLER_H

#include <abstract_task.h>

class CmdHandler : public abstract_task {
private:
  String commands = "<>lLwudsabpmMRc:!";
  String helpText = "Available commands (" + commands +
                    "):\n"
                    "\t-------- DRIVER INFO COMMANDS -----------\n"
                    "\t:<text>  - display driver info text\n"
                    "\t!<text>  - display driver warn text\n"
                    "\tu [off]  - speed up arrow (green)\n"
                    "\td [off]  - speed down arrow (red)\n"
                    "\tR        - reset and reinit display\n"
                    "\t-------- TEST COMMANDS ------------------\n"
                    "\ts ddd    - speed value [0...999]\n"
                    "\ts [f|b]  - drive forward|backward\n"
                    "\ta dd     - acceleration value [0...9]\n"
                    "\tb fff.f  - battary voltage [0...999]\n"
                    "\tp ffff.f - photovoltaics current [-999...+999]\n"
                    "\tm ffff.f - motor current [-999...+999]\n"
                    "\tM ddd    - set motor potentiometer [0...255]\n"
                    "\t< [off]  - left indicator\n"
                    "\t> [off]  - right indicator\n"
                    "\tw [off]  - hazard warning lights\n"
                    "\tl [off]  - position lights on/off\n"
                    "\tL [off]  - beam light on/off\n"
                    "\tc [s|p]  - constant speed|power mode\n"
                    "\t\n";

public:
  void init();
  void re_init();
  string getName(void) { return "CmdHandler"; };
  void exit(void);
  void task(void); // this is the actual task
};

#endif // CMDHANDLER_H
