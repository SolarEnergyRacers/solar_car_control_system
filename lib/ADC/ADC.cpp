//
// Analog to Digital Converter
//

#include <abstract_task.h>
#include <definitions.h>

#include <ADS1X15.h>
#include <I2CBus.h>
#include <Wire.h> // I2C

#include <ADC.h>
#include <DAC.h>
#include <DriverDisplay.h>

extern I2CBus i2cBus;
extern ADC adc;
extern DAC dac;
extern DriverDisplay driverDisplay;

void ADC::init() {

  // instantiate the devices with their corresponding address
  ads_addrs[0] = I2C_ADDRESS_ADS1x15_0;
  ads_addrs[1] = I2C_ADDRESS_ADS1x15_1;
  ads_addrs[2] = I2C_ADDRESS_ADS1x15_2;

  for (int idx = 0; idx < NUM_ADC_DEVICES; idx++) {
    printf("[?] Init 'ADC[%d]' with address 0x%x ...", idx, ads_addrs[idx]);

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
    adss[idx].setDataRate(3);

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
    xSemaphoreGive(i2cBus.mutex);

    char msg[100];
    snprintf(msg, 100, "[v] ADC[%d] at 0x%x initialized.\n", idx, ads_addrs[idx]);
    printf(msg);
    driverDisplay.print(msg);
  }
}

int16_t ADC::read(ADC::Pin port) {

  int idx = port >> 4;
  int pin = port & 0xf;
  debug_printf_l3("index: 0x%x, pin: 0x%x \n", idx, pin);

  xSemaphoreTake(i2cBus.mutex, portMAX_DELAY);
  int16_t value = ADC::adss[idx].readADC(pin);
  xSemaphoreGive(i2cBus.mutex);

  debug_printf_l3("index: 0x%x, pin: 0x%x => value=%d\n", idx, pin, value);
  return value;
}
float ADC::get_multiplier(Pin port) {
  int devNr = port >> 4;
  return adss[devNr].toVoltage(1);
}
