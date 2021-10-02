/*
 * Analog to Digital Converter
 */

#ifndef ADC_H
#define ADC_H

#include <ADS1X15.h> // ADS1x15
#include <definitions.h>

class ADC {
private:
  const int MIN_ADJUST_GAP = 10; // 0...Gap units show 0 (real reachable null point)
  const int MAX_ADJUST_GAP = 10; // max ... max+Gap to avoid max higher then adjusted max
  const int SMOOTHING = 1;
  const int MAX_DISPLAY_VALUE = 99;

  ADS1115 adss[NUM_ADC_DEVICES]; // for ADS1115 use: ADS1115 ads[NUM_ADS_DEVICES];
  int ads_addrs[NUM_ADC_DEVICES];
  int16_t ads_min_dec = 8000;
  int16_t ads_min_acc = 10000;
  int16_t ads_max_dec = 12000;
  int16_t ads_max_acc = 15000;
  int16_t accelLast = 0;
  int16_t recupLast = 0;
  int16_t valueDisplayLast = 0;
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
  void adjust_min_acc_dec();
  void adjust_max_acc_dec();
  int read_adc_acc_dec(void);
  int normalize(int value, int maxValue, int min, int max);
};

#endif // ADC_H
