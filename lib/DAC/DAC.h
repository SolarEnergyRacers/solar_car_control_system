//
// Digital to Analog Converter
//

#ifndef SOLAR_CAR_CONTROL_SYSTEM_DAC_H
#define SOLAR_CAR_CONTROL_SYSTEM_DAC_H

#define DAC_MAX 255

class DAC {
private:
  bool isLocked = true;

public:
  enum pot_chan {
    POT_CHAN0 = 0, // acceleration input
    POT_CHAN1 = 1, // deceleration (recuperation) input
    POT_CHAN_ALL = 2,
  };

private:
  void unlock() { isLocked = false; };
  uint8_t get_cmd(pot_chan channel);

public:
  string getName(void) { return "DAC"; };
  string init();
  string re_init();
  void reset_and_lock_pot();
  void lock();
  void set_pot(uint8_t val, pot_chan channel);
  uint16_t get_pot(pot_chan channel);

  bool verboseModeDAC = false;
};

#endif // SOLAR_CAR_CONTROL_SYSTEM_DAC_H
