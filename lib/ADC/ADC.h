/*
 * Analog to Digital Converter
 */

#ifndef SOLAR_CAR_CONTROL_SYSTEM_ADC_H
#define SOLAR_CAR_CONTROL_SYSTEM_ADC_H

#include <string>

#include <ADS1X15.h>

#include <abstract_task.h>
#include <definitions.h>

class ADC : public abstract_task {

public:
  // RTOS task
  string getName(void) { return "ADC"; };
  string init(void);
  string re_init(void);
  void exit(void);
  void task(void);

  // Class member and functions
private:
  ADS1115 adss[NUM_ADC_DEVICES]; // for ADS1115 use: ADS1115 ads[NUM_ADS_DEVICES];
  int ads_addrs[NUM_ADC_DEVICES];

public:
  enum Pin { // high nibble: device number, low nibble: port
    // ADC0
    MOTOR_SPEED_PORT = 0x00,
    BAT_CURRENT_PORT = 0x01,
    MOTOR_CURRENT_PORT = 0x02,
    PV_CURRENT_PORT = 0x03,
    // ADC1
    BAT_VOLTAGE_PORT = 0x10,
    MOTOR_VOLTAGE_PORT = 0x11,
    REFERENCE_CELL_PORT = 0x12,
    GREEN_LIGHT_PORT = 0x13,
    // ADC2 (steering wheel)
    STW_ACC_PORT = 0x20,
    STW_DEC_PORT = 0x21,
    Reserve_22_PORT = 0x22,
    Reserve_23_PORT = 0x23
  };
  // ADC native values
  int16_t MOTOR_SPEED;
  int16_t BAT_CURRENT;
  int16_t MOTOR_CURRENT;
  int16_t PV_CURRENT;
  // ADC1
  int16_t BAT_VOLTAGE;
  int16_t MOTOR_VOLTAGE;
  int16_t REFERENCE_CELL;
  int16_t GREEN_LIGHT;
  // ADC2 (steering wheel)
  int16_t STW_ACC;
  int16_t STW_DEC;

  int16_t read(Pin port);
  float get_multiplier(Pin port);
  bool verboseModeADC = false;
};

#endif // SOLAR_CAR_CONTROL_SYSTEM_ADC_H
