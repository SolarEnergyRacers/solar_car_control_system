//
// Car Speed PID Control
//

#include <definitions.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <inttypes.h>
#include <iostream>
#include <stdio.h>

//#include <Arduino.h>

#include <CarSpeed.h>
#include <CarState.h>
#include <Console.h>
#include <DAC.h>
#include <Helper.h>
#include <PID_v1.h>

extern Console console;
extern PID pid;
extern CarSpeed carSpeed;
// extern ADC adc;
extern DAC dac;
extern CarState carState;

// ------------------
// FreeRTOS functions

void CarSpeed::re_init() { init(); }

void CarSpeed::init() {
  target_speed = 0;
  pid = PID(&input_value, &output_setpoint, &target_speed, Kp, Ki, Kd, DIRECT);
  pid.SetMode(AUTOMATIC);
  sleep_polling_ms = 400;
  console << "[v]" << getName() << " initialized.\n";
}

void CarSpeed::exit(void) { set_target_speed(0); }
// ------------------

void CarSpeed::set_target_speed(double speed) { target_speed = speed; }

void CarSpeed::target_speed_incr(void) { target_speed += speed_increment; }

void CarSpeed::target_speed_decr(void) { target_speed -= speed_increment; }

double CarSpeed::get_target_speed() { return target_speed; }

double CarSpeed::get_current_speed() {
  // TODO: this function can be heavily optimized (i.e. use int instead of float, use less arithmetic operations

  // read analog value
  // TODO: Which methode?
  // int16_t value = adc.read(ADC::Pin::MOTOR_SPEED);
  int16_t value = carState.Speed;

  // convert value to voltage
  float voltage = value * 5.0 / 65536; // TODO: check if we really use 16bit ADS1115

  // convert value from voltage to revolutions per minute: 370rpm / V
  float wheel_circumference = 1.0; // TODO: set circumference = diameter*pi, in meters
  return (double)370 * wheel_circumference / 60.0;
}

void CarSpeed::update_pid(double Kp, double Ki, double Kd) { pid.SetTunings(Kp, Ki, Kd); }

void CarSpeed::task() {

  /*
   * How is accelration / decelartion handled in hardware?
   * - acceleration: Digital to analog converter value representing the target speed -> 0V: max rev, 2.5V: stop, 5V: max fwd
   * - deceleration: DIgital to analog converter value representing the recuperation: -> 0V: no rec, 5V: max recup
   * -> in case of recup > 0, we should have acceleration on 2.5V
   *
   */

  // polling loop
  while (1) {
    if (carState.ConstantModeOn && carState.ConstantMode == CONSTANT_MODE::SPEED) {
      // read target speed
      input_value = carState.Speed;
      target_speed = carState.TargetSpeed; // can be negative for deceleration

      // update pid controller
      pid.Compute();

      // check range
      if (output_setpoint < -DAC_MAX)
        output_setpoint = -DAC_MAX;
      if (output_setpoint > DAC_MAX)
        output_setpoint = DAC_MAX;

      if (carState.TargetRecuperation > 0) { // we recuperate
        dac.set_pot(carState.TargetRecuperation, DAC::pot_chan::POT_CHAN1);

      } else { // we accelerate  fwd/backwrd
        // set acceleration & deceleration
        if (output_setpoint > 0) {
          // TODO: set MC switch to forward
        } else {
          // TODO: set MC switch to reverse
        }
        dac.set_pot(output_setpoint, DAC::pot_chan::POT_CHAN0); // acceleration forward or backward
        console << "#--- input_value=" << input_value << ", target_speed=" << target_speed << " ==> deceleration=" << output_setpoint
                << "\n";
      }
    }
    // sleep
    vTaskDelay(sleep_polling_ms / portTICK_PERIOD_MS);
  }
}
