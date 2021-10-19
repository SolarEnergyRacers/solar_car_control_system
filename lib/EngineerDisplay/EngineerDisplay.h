//
// Display
//

#ifndef SER_ENGINEER_DISPLAY_H
#define SER_ENGINEER_DISPLAY_H

#include <Display.h>
#include <Wire.h>
#include <abstract_task.h>
#include <definitions.h>

class EngineerDisplay : public Display {
private:
  //==== Driver Display definitions ==== START
  // display formats and sizes
  int bgColor = ILI9341_LIGHTGREY;
  int U1FrameX = 10;
  int U1FrameY = 10;
  int I1FrameX = 10;
  int I1FrameY = 40;
  int I2FrameX = 10;
  int I2FrameY = 60;
  //==== Driver Display definition ==== END

public:
  string getName(void) { return "EngineerDisplay"; };

  //==== overwrites from base class ==== START
  DISPLAY_STATUS display_setup(DISPLAY_STATUS status) override;
  DISPLAY_STATUS task(DISPLAY_STATUS status, int lifeSignCounter) override;
  //==== overwrites from base class ==== END

  void print(string msg);
  void draw_display_background();
};

#endif // SER_ENGINEER_DISPLAY_H DISPLAY_H
