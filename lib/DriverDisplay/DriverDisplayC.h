//
// Display
//
#include "../../include/LocalFunctionsAndDevices.h"

#ifndef DRIVER_DISPLAY_C_H
#define DRIVER_DISPLAY_C_H

#define ILI9341 // (320x240)

#include "../../interfaces/abstract_task.h"
#include <Adafruit_ILI9341.h> // placed here for display colors in other moduls
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

using namespace std;

extern SemaphoreHandle_t spi_mutex;

// namespace DriverDisplayC {
// public structures
enum class INDICATOR { OFF, LEFT, RIGHT, WARN };
enum class INFO_TYPE { INFO, STATUS, WARN, ERROR };
enum class CONSTANT_MODE { SPEED, POWER };
enum class DRIVE_DIRECTION { FORWARD, BACKWARD };

class DriverDisplayC : public abstract_task {

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
  static DriverDisplayC *_instance;

  // put private/internal variables/functions here
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
  void _turn_Left(int);
  void _turn_Right(int);
  bool init_driver_display(void);

public:
  string getName(void);

  void re_init(void);
  void exit(void);

  // RTOS task
  void init(void);

  // public functions
  void draw_display_border(int color);
  void draw_speed_border(int);
  void draw_acceleration_border(int);
  void lifeSign();
  void draw_display_background();

  void write_constant_mode(CONSTANT_MODE);
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
