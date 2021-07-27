//
// Display
//
#include "LocalFunctionsAndDevices.h"

#ifndef DRIVER_DISPLAY_C_H
#define DRIVER_DISPLAY_C_H

#define ILI9341 // (320x240)

#include <ADS1X15.h>          // ADS1x15
#include <Adafruit_ILI9341.h> // placed here for display colors in other moduls
#include <abstract_task.h>

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

using namespace std;

extern SemaphoreHandle_t spi_mutex;

// namespace DriverDisplayC {
// public structures
enum class INDICATOR { OFF, LEFT, RIGHT, WARN };
enum class INFO_TYPE { INFO, STATUS, WARN, ERROR };
enum class CONSTANT_MODE { NONE, SPEED, POWER };
enum class DRIVE_DIRECTION { FORWARD, BACKWARD };

class DriverDisplayC : public abstract_task {

private:
  //==== Driver Display definition ==== START
  // display formats and sizes
  int bgColor = 0x000000;
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

  // light on indicator
  int light1OnX = 250;
  int light1OnY = 118;
  int light2OnX = 250;
  int light2OnY = 138;

  // life sign for connection to microprocessor via rtx
  int lifeSignX = -1;
  int lifeSignY = -1;
  int lifeSignRadius = 4;
  //==== Driver Display definition ==== END

public:
  // INFO:ILI9341_WHITE, STATUS:ILI9341_GREEN,
  // WARN.ILI9341_PURPLE, ERROR.ILI9341_RED

  static DriverDisplayC *instance() {
    if (!_instance) {
      _instance = new DriverDisplayC();
    }
    return _instance;
  }
  virtual ~DriverDisplayC() { _instance = 0; }

protected:
  DriverDisplayC() {}

private:
  enum class DISPLAY_STATUS { SETUP, DISPLAY_DEMOSCREEN, DISPLAY_BACKGROUND, WORK };
  template <typename Enumeration> auto as_integer(Enumeration const value) -> typename std::underlying_type<Enumeration>::type {
    return static_cast<typename std::underlying_type<Enumeration>::type>(value);
  }
  static DriverDisplayC *_instance;
  static DISPLAY_STATUS status;

  // put private/internal variables/functions here
  void _setup(void);
  uint32_t sleep_polling_ms = 1000;
  float _write_float(int x, int y, float valueLast, float value, int textSize, int color);
  int _write_ganz_99(int x, int y, int valueLast, int value, int textSize, int color);
  int _write_nat_999(int x, int y, int valueLast, int value, int textSize, int color);
  void _arrow_increase(int color);
  void _arrow_decrease(int color);
  void _light1(bool lightOn);
  void _light2(bool lightOn);
  void _drawCentreString(const String &buf, int x, int y);
  int _getColorForInfoType(INFO_TYPE type);
  void _turn_Left(int color);
  void _turn_Right(int color);
  bool init_driver_display(void);

public:
  string getName(void);

  // RTOS task
  void init(void);
  void re_init(void);
  void exit(void);

  // public functions
  void draw_display_border(int color);
  void draw_speed_border(int);
  void draw_acceleration_border(int);
  void lifeSign();
  void draw_display_background();

  void constant_drive_mode_set(CONSTANT_MODE mode);
  void constant_drive_on();
  void constant_drive_off();
  void constant_drive_mode_show();
  void constant_drive_mode_hide();

  void write_drive_direction(DRIVE_DIRECTION);
  void write_driver_info(String msg, INFO_TYPE type);
  void write_speed(int speed);
  void write_bat(float voltage);
  void write_motor(float ampers);
  void write_pv(float voltage);
  void write_acceleration(int value);

  void indicator_set_and_blink(INDICATOR direction);
  void light1OnOff();
  void light2OnOff();

  void speedCheck(int speed);
  void arrow_increase(bool on);
  void arrow_decrease(bool on);

  void driver_display_demo_screen();

  // internal functions for inner task communication
  INDICATOR getIndicatorDirection();
  void indicator_set_and_blink(INDICATOR direction, bool blinkOn);
  bool getIndicatorState();
  void setIndicatorState(bool state);
  void task(void);
};
//} // namespace DriverDisplayC
#endif // DRIVER_DISPLAY_C_H
