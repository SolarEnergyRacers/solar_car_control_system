//
// Car Speed PID Control
//

#ifndef SOLAR_CAR_CONTROL_SYSTEM_CARSPEED_H
#define SOLAR_CAR_CONTROL_SYSTEM_CARSPEED_H

#include <CarState.h>
#include <PID_v1.h>
#include <abstract_task.h>
#include <stdio.h>

extern CarState carState;

class CarSpeed : public abstract_task {
public:
  // RTOS task
  string getName(void) { return "CarSpeed"; };
  string init(void);
  string re_init(void);
  void exit(void);
  void task(void);

  // Class functions and members
private:
  double input_value;
  double output_setpoint;
  double target_speed;          // in m/s
  double speed_increment = 1.0; // in m/s
  PID pid = PID(&input_value, &output_setpoint, &target_speed, carState.Kp, carState.Ki, carState.Kd, DIRECT);

public:
  void set_target_speed(double speed);
  double get_target_speed();
  double get_current_speed(); // speed in m/s
  void update_pid(double Kp, double Ki, double Kd);
  void target_speed_incr(void);
  void target_speed_decr(void);
  double GetKp() { return pid.GetKp(); }
  double GetKi() { return pid.GetKi(); }
  double GetKd() { return pid.GetKd(); }
  bool verboseModePID = false;
};

#endif // SOLAR_CAR_CONTROL_SYSTEM_CARSPEED_H
