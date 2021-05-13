//
// Digital to Analog Converter
//

#ifndef SOLAR_CAR_CONTROL_SYSTEM_DAC_H
#define SOLAR_CAR_CONTROL_SYSTEM_DAC_H

enum pot_chan {
  POT_CHAN0 = 0,
  POT_CHAN1 = 1,
  POT_CHAN_ALL = 2,
};

void init_dac(void);
void set_pot(uint8_t val, pot_chan channel);
uint16_t get_pot(pot_chan channel);

#endif // SOLAR_CAR_CONTROL_SYSTEM_DAC_H
