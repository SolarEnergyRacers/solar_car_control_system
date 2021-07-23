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
  ADS1115 adss[NUM_ADC_DEVICES]; // for ADS1115 use: ADS1115 ads[NUM_ADS_DEVICES];
  int ads_addrs[NUM_ADC_DEVICES];
  int16_t ads_min_recuperation = 8000;
  int16_t ads_min_acceleration = 10000;
  int16_t ads_max_recuperation = 12000;
  int16_t ads_max_acceleration = 15000;
  int minAdjustGap = 10; // 0...Gap units show 0 (real reachable null point)
  int maxAdjustGap = 10; // max ... max+Gap to avoid max higher then adjusted max
  int16_t accelLast = 0;
  int16_t recupLast = 0;
  int16_t accDisplayLast = 0;
  bool justInited = true;

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
  void adjustMin_acceleration_recuperation();
  void adjustMax_acceleration_recuperation();
  int read_adc_acceleration_recuperation(void);
  int normalize(int value, int maxValue);
  int normalize2(int value, int maxValue, int min, int max);
};

#endif // ADC_H
