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
  double Ki = 1;
  double Kd = 0.1;
  double speed_increment = 1.0;
  PID pid = PID(&input_value, &output_setpoint, &target_speed, Kp, Ki, Kd, DIRECT);
  // if (output_setpoint < 0) {
  //   carState.Acceleration = output_setpoint; // acceleration
  //   carState.Deceleration = 0;               // deceleration
  //   cout << "#+++ input_value=" << input_value << ", target_speed=" << target_speed << " ==> Acceleration=" << output_setpoint << endl;
  // } else {
  //   carState.Acceleration = 0;                // acceleration
  //   carState.Deceleration = -output_setpoint; // deceleration
  //   cout << "#--- input_value=" << input_value << ", target_speed=" << target_speed << " ==> deceleration=" << output_setpoint << endl;
  // }
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
