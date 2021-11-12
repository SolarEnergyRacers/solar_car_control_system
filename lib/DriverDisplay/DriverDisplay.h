//
// DriverDisplay
//

#ifndef SER_DRIVER_DISPLAY_C_H
#define SER_DRIVER_DISPLAY_C_H

#define ILI9341 // (320x240)

#include <ADS1X15.h>
#include <Adafruit_ILI9341.h> // placed here for display colors in other moduls

#include <CarState.h>
#include <Display.h>
#include <DisplayValue.h>
#include <LocalFunctionsAndDevices.h>

using namespace std;

class DriverDisplay : public Display {
public:
  DriverDisplay() { bgColor = ILI9341_BLACK; };
  ~DriverDisplay(){};
  //==== overwrites from base class ==== START
  string getName() { return "DriverDisplay"; };

private:
  DisplayValue<float> MotorCurrent = DisplayValue<float>(10, 180, "Motor:", "%5.1f", "A", ILI9341_ORANGE, ILI9341_BLACK);
  DisplayValue<bool> MotorOn = DisplayValue<bool>(160, 180, "-", "%3s", "", ILI9341_MAROON, ILI9341_BLACK);
  DisplayValue<float> BatteryVoltage = DisplayValue<float>(10, 200, "Bat  :", "%5.1f", "V", ILI9341_ORANGE, ILI9341_BLACK);
  DisplayValue<bool> BatteryOn = DisplayValue<bool>(160, 200, "-", "%3s", "", ILI9341_MAROON, ILI9341_BLACK);
  DisplayValue<float> PhotoVoltaicCurrent = DisplayValue<float>(10, 220, "PV   :", "%5.1f", "A", ILI9341_ORANGE, ILI9341_BLACK);
  DisplayValue<bool> PhotoVoltaicOn = DisplayValue<bool>(160, 220, "-", "%3s", "", ILI9341_MAROON, ILI9341_BLACK);

  //==== display cache =====================
  // ... to avoid flickering
  int speedLast = -1;
  int accelerationLast = -1;
  bool blinkOn = true;
  bool justInited = true;
  //=======================================

  //==== Driver Display definitions ==== START
  // display formats and sizes
  int infoFrameX = 0;
  int infoFrameY = 0;
  int infoFrameSizeX = -1; // full tft width, calculated beow
  int infoFrameSizeY = 64;
  int infoTextSize = 3;

  // frame around value display (exclude text message lines)
  int mainFrameX = infoFrameSizeY;

  // speed display
  int speedFrameX = -1; // get calculated later: (sizeX - speedFrameCx) / 2;
  int speedFrameY = 80;
  int speedFrameSizeX = 156;
  int speedFrameSizeY = 76;
  int speedTextSize = 8;

  // acceleration display
  int accFrameX = 2;
  int accFrameY = 118;    // speedFrameY + speedFrameSizeY / 2
  int accFrameSizeX = -1; // get calculated later: speedFrameX - 4
  int accFrameSizeY = 38; // speedFrameSizeY / 2
  int accTextSize = 4;    // speedTextSize / 2

  // ---- voltage and current displays ---- START
  int labelLen = 9; // label length for all 3 voltage/current displays

  // battery voltage display
  int batFrameX = 10;
  int batFrameY = 180;
  int batTextSize = 2;
  int lightTextSize = 2;

  // photovoltaics voltage display
  int pvFrameX = 10;
  int pvFrameY = 200;
  int pvTextSize = 2;

  // motor current display
  int motorFrameX = 10;
  int motorFrameY = 220;
  int motorTextSize = 2;
  // ---- voltage and current displays ---- END

  // constant mode speed or power display
  int constantModeX = 250;
  int constantModeY = 158;
  int constantModeTextSize = 2;

  // constant mode speed or power display
  int driveDirectionX = 220;
  int driveDirectionY = 178;
  int driveDirectionTextSize = 2;

  // turn indicator arrows
  int indicatorLeftX = 10;
  int indicatorY = 92;
  int indicatorRightX = 310;
  int indicatorWidth = 30;
  int indicatorHeight = 20;

  int lightX = 250;
  int lightY = 138;
  //==== Driver Display definition ==== END

protected:
  DISPLAY_STATUS display_setup(DISPLAY_STATUS status) override;
  DISPLAY_STATUS task(DISPLAY_STATUS status, int lifeSignCounter) override;
  //==== overwrites from base class ==== END

private:
  void _arrow_increase(int color);
  void _arrow_decrease(int color);
  // void _light1(bool lightOn);
  // void _light2(bool lightOn);
  void _hide_light();
  void _turn_Left(int color);
  void _turn_Right(int color);
  bool init_driver_display(void);
  void draw_display_border(int color);
  void draw_speed_border(int color);
  void draw_acceleration_border(int color);
  void draw_display_background();

  void constant_drive_mode_show();

  void write_drive_direction();
  void write_driver_info();
  void write_speed();
  void write_acceleration();

  void show_indicator();
  void show_light();

  void speedCheck(int speed);
  void arrow_increase(bool on);
  void arrow_decrease(bool on);

  void driver_display_demo_screen();
  int getColorForInfoType(INFO_TYPE type);
};

#endif // #ifndef SER_DRIVER_DISPLAY_C_H
