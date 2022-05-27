//
// Car Speed PID Control
//

#ifndef SOLAR_CAR_CONTROL_SYSTEM_CARSPEED_H
#define SOLAR_CAR_CONTROL_SYSTEM_CARSPEED_H

#include <PID_v1.h>
#include <abstract_task.h>
#include <stdio.h>

class CarSpeed : public abstract_task {
private:
  uint32_t sleep_polling_ms = 400;

public:
  // RTOS task
  void set_SleepTime(uint32_t milliseconds) { sleep_polling_ms = milliseconds; };
  uint32_t get_SleepTime() { return sleep_polling_ms; };
  string getName(void) { return "CarSpeed"; };
  void init(void);
  void re_init(void);
  void exit(void);
  void task(void);

  // Class functions and members
private:
  double input_value;
  double output_setpoint;
  double target_speed;
  double Kp = 2;
  double Ki = 1;
  double Kd = 0.1;
  double speed_increment = 1.0;
  PID pid = PID(&input_value, &output_setpoint, &target_speed, Kp, Ki, Kd, DIRECT);

public:
  void set_target_speed(double speed);
  double get_target_speed();
  double get_current_speed(); // speed in m/s
  void update_pid(double Kp, double Ki, double Kd);
  void target_speed_incr(void);
  void target_speed_decr(void);
};

#endif // SOLAR_CAR_CONTROL_SYSTEM_CARSPEED_H
