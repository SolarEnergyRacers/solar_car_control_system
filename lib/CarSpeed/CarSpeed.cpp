//
// Car Speed PID Control
//

#include <definitions.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <fmt/core.h>
#include <fmt/format.h>
#include <inttypes.h>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>

#include <CarControl.h>
#include <CarSpeed.h>
#include <CarState.h>

#include <Console.h>
#include <DAC.h>
#include <Helper.h>
#include <PID_v1.h>

extern Console console;
extern PID pid;
extern CarSpeed carSpeed;
extern CarState carState;
#if DAC_ON
extern DAC dac;
#endif

// ------------------
// FreeRTOS functions

string CarSpeed::re_init() { return init(); }

string CarSpeed::init() {
  bool hasError = false;
  console << "[  ] Init 'CarSpeed'...\n";
  target_speed = 0;
  pid = PID(&input_value, &output_setpoint, &target_speed, carState.Kp, carState.Ki, carState.Kd, DIRECT);
  pid.SetMode(AUTOMATIC);
  pid.SetOutputLimits(-DAC_MAX, DAC_MAX);
  return fmt::format("[{}] CarSpeed initialized.", hasError ? "--" : "ok");
}

void CarSpeed::exit(void) { set_target_speed(0); }
// ------------------

void CarSpeed::set_target_speed(double speed) { target_speed = speed; }

void CarSpeed::target_speed_incr(void) { target_speed += speed_increment; }

void CarSpeed::target_speed_decr(void) { target_speed -= speed_increment; }

double CarSpeed::get_target_speed() { return target_speed; }

double CarSpeed::get_current_speed() { return carState.Speed; }

void CarSpeed::update_pid(double Kp, double Ki, double Kd) {
  carState.Kp = Kp;
  carState.Ki = Ki;
  carState.Kd = Kd;
  pid.SetTunings(carState.Kp, carState.Ki, carState.Kd);
}

void CarSpeed::task() {

  /*
   * How is accelration / decelartion handled in hardware?
   *
   * - acceleration: Digital to analog converter value representing the target speed -> 0V: stop, 5V -> max speed
   * - a separate I/O is used for forward/reverse switching
   * - deceleration/recuperation: Digital to analog converter value representing the recuperation amount: -> 0V: no rec, 5V: max recup
   * -> Note: In case of recup > 0, we should have acceleration 0
   *
   * TODO: ini file: recuperate on constant speed mode , or just let it roll (i.e. let it roll if the speed is too high is less convenient
   * for the driver, however, it conserves energy since we do not over-regulate) For the moment, we recuperate
   */

  // polling loop
  while (1) {
    if (carState.ConstantModeOn && carState.ConstantMode == CONSTANT_MODE::SPEED) {
      // read target speed
      input_value = carState.Speed;
      target_speed = carState.TargetSpeed;

      stringstream ss("#- input_value=");
      ss << input_value << ", target_speed=" << target_speed << " =>";
      // update pid controller
      bool hasNewValue = pid.Compute();
      if (!hasNewValue) {
        if (verboseModePID) {
          ss << " cst=0" << carState.AccelerationDisplay << "(" << output_setpoint << ")\n";
        }
        return;
      }

      // set acceleration & deceleration
      uint8_t acc = 0;
      uint8_t dec = 0;
      if (output_setpoint > 0) {
        acc = round(output_setpoint);
        ss << "acc=" << acc;
      } else if (output_setpoint < 0) {
        dec = round(-output_setpoint);
        ss << "dec=" << dec;
      }
#if DAC_ON
      dac.set_pot(acc, DAC::pot_chan::POT_CHAN0); // acceleration
      dac.set_pot(dec, DAC::pot_chan::POT_CHAN1); // deceleration
#endif
      carState.AccelerationDisplay = round((acc > 0 ? acc : -dec) * MAX_ACCELERATION_DISPLAY_VALUE / DAC_MAX);
      if (verboseModePID) {
        ss << carState.AccelerationDisplay << " (" << output_setpoint << ")\n";
      }
    }
    // sleep
    vTaskDelay(sleep_polling_ms / portTICK_PERIOD_MS);
  }
}
