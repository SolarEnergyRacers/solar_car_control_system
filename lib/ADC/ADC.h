/*
 * Analog to Digital Converter
 */

#ifndef SOLAR_CAR_CONTROL_SYSTEM_ADC_H
#define SOLAR_CAR_CONTROL_SYSTEM_ADC_H

#include <ADS1X15.h>
#include <definitions.h>

class ADC {
private:
  ADS1115 adss[NUM_ADC_DEVICES]; // for ADS1115 use: ADS1115 ads[NUM_ADS_DEVICES];
  int ads_addrs[NUM_ADC_DEVICES];

public:
  enum Pin { // high nibble: device number, low nibble: port
    // ADC0
    MOTOR_SPEED = 0x00,
    BAT_CURRENT = 0x01,
    MOTOR_CURRENT = 0x02,
    PV_CURRENT = 0x03,
    // ADC1
    BAT_VOLTAGE = 0x10,
    MOTOR_VOLTAGE = 0x11,
    // ADC2 (steering wheel)
    STW_ACC = 0x20,
    STW_DEC = 0x21
  };
  void init();
  int16_t read(Pin port);
  float get_multiplier(Pin port);
};

#endif // SOLAR_CAR_CONTROL_SYSTEM_ADC_H
