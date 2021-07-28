//
// Digital to Analog Converter
//

#ifndef SOLAR_CAR_CONTROL_SYSTEM_DAC_H
#define SOLAR_CAR_CONTROL_SYSTEM_DAC_H

class DAC {
private:
public:
  enum pot_chan {
    POT_CHAN0 = 0, // acceleration input
    POT_CHAN1 = 1, // deceleration (recuperation) input
    POT_CHAN_ALL = 2,
  };
  void init();
  void re_init();
  void set_pot(uint8_t val, pot_chan channel);
  uint16_t get_pot(pot_chan channel);
  uint8_t get_cmd(pot_chan channel);
};

#endif // SOLAR_CAR_CONTROL_SYSTEM_DAC_H
