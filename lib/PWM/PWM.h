//
// 16-Channel Pulse Width Modulation
//

#ifndef SOLAR_CAR_CONTROL_SYSTEM_PWM_H
#define SOLAR_CAR_CONTROL_SYSTEM_PWM_H

#include <Adafruit_PWMServoDriver.h>
#include <abstract_task.h>

class PWM : public abstract_task {
private:
  uint32_t sleep_polling_ms;

public:
  void set_SleepTime(uint32_t milliseconds) { sleep_polling_ms = milliseconds; };
  uint32_t get_SleepTime() { return sleep_polling_ms; }
  // RTOS task
  string getName(void) { return "PWM"; };
  string init(void);
  string re_init(void);
  void exit(void);
  void task(void);

  // Class functions and members
private:
  Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(I2C_ADDRESS_PCA9685, Wire);

public:
  void update_pwm(int channel, int value);
};

#endif // SOLAR_CAR_CONTROL_SYSTEM_PWM_H
