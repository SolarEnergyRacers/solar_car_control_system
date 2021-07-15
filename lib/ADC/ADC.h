/*
 * Analog to Digital Converter
 */

#ifndef ADC_H
#define ADC_H

#include <ADS1X15.h> // ADS1x15
#include <definitions.h>

// void read_adc_acceleration_recuperation(void *pvParameter); // adc does not need its own task, therefore we let this demo task outside of
//                                                             // the class structure instead of extending the abstract task class

class ADC {
private:
  ADS1015 adcs[NUM_ADC_DEVICES]; // for ADS1115 use: ADS1115 ads[NUM_ADS_DEVICES];
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
  void re_init();
  int16_t read(Pin port);
  int read_adc_acceleration_recuperation(void);
  int normalize(int value, int maxValue);
  int normalize2(int value, int maxValue, int min, int max);
};

#endif // ADC_H
