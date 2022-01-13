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
  double input_value;
  double output_setpoint;
  double target_speed;
  double Kp = 2;
  double Ki = 5;
  double Kd = 1;
  double speed_increment = 1.0;
  PID pid = PID(&input_value, &output_setpoint, &target_speed, Kp, Ki, Kd, DIRECT);

public:
  string getName(void) { return "CarSpeed"; };
  void init(void);
  void re_init(void);
  void exit(void);
  void task(void);
  void set_target_speed(double speed);
  double get_target_speed();
  double get_current_speed(); // speed in m/s
  void update_pid(double Kp, double Ki, double Kd);
  void target_speed_incr(void);
  void target_speed_decr(void);
};

#endif // SOLAR_CAR_CONTROL_SYSTEM_CARSPEED_H
