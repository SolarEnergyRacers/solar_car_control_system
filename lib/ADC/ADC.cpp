//
// Analog to Digital Converter
//

#include <abstract_task.h>
#include <definitions.h>

#include <ADS1X15.h> // ADS1x15
#include <I2CBus.h>
#include <Wire.h> // I2C

#include <ADC.h>
#include <DAC.h>
#include <DriverDisplayC.h>

extern I2CBus i2cBus;
extern ADC adc;
extern DAC dac;

void ADC::re_init() { ADC::init(); }

void ADC::init() {

  // instantiate the devices with their corresponding address
  ads_addrs[0] = 0; // I2C_ADDRESS_ADS1x15_0;
  ads_addrs[1] = 0; // I2C_ADDRESS_ADS1x15_1;
  ads_addrs[2] = I2C_ADDRESS_ADS1x15_2;

  for (int idx = 0; idx < NUM_ADC_DEVICES; idx++) {
    if (ads_addrs[idx] == 0) {
      continue;
    }
    printf("    Init 'ADC[%d]' with address 0x%x ...", idx, ads_addrs[idx]);

    xSemaphoreTake(i2cBus.mutex, portMAX_DELAY);
    adss[idx] = ADS1115(ads_addrs[idx]);

    bool result = adss[idx].begin();
    if (!result) {
      printf(" ERROR: Wrong ADS1x15 address 0x%x.\n", ads_addrs[idx]);
      xSemaphoreGive(i2cBus.mutex);
      continue;
    }
    printf("\n");
    // set gain amplifier value
    // 2/3x gain +/- 6.144V
    // 1 bit = 3mV (ADS1015) / 0.1875mV (ADS1115)
    adss[idx].setGain(0);
    adss[idx].setMode(1);

    // conversion factor:
    // bit-value -> mV: 2/3x gain +/- 6.144V
    // 1 bit = 3mV (ADS1015) 0.1875mV (ADS1115)
    float multiplier = adss[idx].toVoltage(1); // voltage factor
    printf("    Max voltage=%f with multiplier=%f\n", adss[idx].getMaxVoltage(), multiplier);
    // read all inputs & report
    for (int i = 0; i < 4; i++) {
      int16_t value = adss[idx].readADC(i);
      printf("      [ADS1x15] AIN%d --> %d: %fmV\n", i, value, multiplier * value);
    }
    adss[idx].setDataRate(3);
    xSemaphoreGive(i2cBus.mutex);

    justInited = true;
    printf("[v] ADC[%d] initialized.\n", idx);
  }
}

int16_t ADC::read(ADC::Pin port) {

  int idx = port >> 4;
  int pin = port & 0xf;

  xSemaphoreTake(i2cBus.mutex, portMAX_DELAY);
  int16_t value = ADC::adss[idx].readADC(pin);
  xSemaphoreGive(i2cBus.mutex);

  debug_printf_l2("index: 0x%x, pin: 0x%x => value=%d\n", idx, pin, value);
  return value;
}

void ADC::adjust_min_acc_dec() {
  int16_t valueDec = adc.read(ADC::Pin::STW_DEC);
  int16_t valueAcc = adc.read(ADC::Pin::STW_ACC);
  // filter artefacts
  ads_min_dec = valueDec <= MIN_ADJUST_GAP ? 0 : valueDec - MIN_ADJUST_GAP;
  ads_min_acc = valueAcc <= MIN_ADJUST_GAP ? 0 : valueAcc - MIN_ADJUST_GAP;
  valueDisplayLast = 0;
  debug_printf("Set MIN values set dec: %d-->%d, acc: %d-->%d\n", valueDec, ads_min_dec, valueAcc, ads_min_acc);
}

void ADC::adjust_max_acc_dec() {
  int16_t valueDec = adc.read(ADC::Pin::STW_DEC);
  int16_t valueAcc = adc.read(ADC::Pin::STW_ACC);
  // filter artefacts
  ads_max_dec = valueDec >= 32767 + MAX_ADJUST_GAP ? 32767 : valueDec + MAX_ADJUST_GAP;
  ads_max_acc = valueAcc >= 32767 + MAX_ADJUST_GAP ? 32767 : valueAcc + MAX_ADJUST_GAP;
  valueDisplayLast = 0;
  debug_printf("Set MAX values dec: %d-->%d, acc: %d-->%d\n", valueDec, ads_min_dec, valueAcc, ads_min_acc);
}

int ADC::normalize(int value, int maxValue, int min, int max) {
  int delta = max - min;
  int val0based = value - min;
  int retValue = (int)(val0based * maxValue / delta);
  if (abs(retValue) < (int)(MAX_DISPLAY_VALUE / 10)) {
    retValue = 0;
  }
  return retValue;
}

int ADC::read_adc_acc_dec() {
  int16_t valueAccNorm = 0;
  int16_t valueDecNorm = 0;
  int16_t valueDisplay = 0;

  if (justInited) {
    adc.adjust_min_acc_dec();
    justInited = false;
  }

  int16_t valueDec = adc.read(ADC::Pin::STW_DEC);
  int16_t valueAcc = adc.read(ADC::Pin::STW_ACC);

  if (valueAcc < 0) {
    // filter artefacts
    valueAcc = ads_min_acc;
  }
  if (valueDec < 0) {
    // filter artefacts
    valueDec = ads_min_dec;
  }

  // if (valueDec < ads_min_dec || valueDec > ads_max_dec) {
  //   // filter artefacts
  //   valueDec = recupLast;
  // }

  // if (valueAcc < ads_min_acc || valueAcc > ads_max_acc) {
  //   // filter artefacts
  //   valueAcc = accelLast;
  // }

  // // readjust paddle area
  // if (ads_min_dec > valueDec) {
  //   debug_printf("Adopt ads_min_dec (%5d) to valueDec (%5d - %d)\n", ads_min_dec, valueDec, MIN_ADJUST_GAP);
  //   ads_min_dec = valueDec - MIN_ADJUST_GAP;
  // }
  // if (ads_max_dec < valueDec) {
  //   debug_printf("Adopt ads_max_dec (%5d) to valueDec (%5d + %d)\n", ads_max_dec, valueDec, MAX_ADJUST_GAP);
  //   ads_max_dec = valueDec + MAX_ADJUST_GAP;
  // }
  // if (ads_min_acc > valueAcc) {
  //   debug_printf("Adopt ads_min_acc (%5d) to valueDec (%5d - %d)\n", ads_min_acc, valueAcc, MIN_ADJUST_GAP);
  //   ads_min_acc = valueAcc - MIN_ADJUST_GAP;
  // }
  // if (ads_max_acc < valueAcc) {
  //   debug_printf("Adopt ads_max_acc (%5d) to valueAcc (%5d + %d)\n", ads_max_acc, valueAcc, MAX_ADJUST_GAP);
  //   ads_max_acc = valueAcc + MAX_ADJUST_GAP;
  // }

  valueDecNorm = adc.normalize(valueDec, MAX_DISPLAY_VALUE, ads_min_dec, ads_max_dec);
  if (valueDecNorm > 0) {
    valueAccNorm = 0;
    valueDisplay = -valueDecNorm;
  } else {
    valueAccNorm = adc.normalize(valueAcc, MAX_DISPLAY_VALUE, ads_min_acc, ads_max_acc);
    valueDisplay = valueAccNorm;
  }
  valueDisplay = (int)((valueDisplayLast * (SMOOTHING - 1) + valueDisplay) / SMOOTHING);

  // prepare and write motor acceleration and recuperation values to DigiPot
  int valueDecPot = 0;
  int valueAccPot = 0;
  if (valueDisplay < 0) {
    valueDecPot = -(int)(((float)valueDisplay / MAX_DISPLAY_VALUE) * 1024);
  } else {
    valueAccPot = (int)(((float)valueDisplay / MAX_DISPLAY_VALUE) * 1024);
  }

  if (valueDisplayLast != valueDisplay) {
    debug_printf("Dec (v0):  %5d --> %3d | Acc (v1): %5d --> %3d | "
                 "ACCEL-DISPLAY: %3d"
                 " ==> set POT0 =%3d (dec(%5d-%5d)), POT1 =%3d (acc(%5d-%5d))\n",
                 valueDec, valueDecNorm, valueAcc, valueAccNorm, valueDisplay, valueDecPot, ads_min_dec, ads_max_dec, valueAccPot,
                 ads_min_acc, ads_max_acc);

    valueDisplayLast = valueDisplay;
    dac.set_pot(valueAccPot, DAC::pot_chan::POT_CHAN0);
    dac.set_pot(valueDecPot, DAC::pot_chan::POT_CHAN1);
  }

  return valueDisplayLast;
}
