//
// Car Speed PID Control
//

#include <definitions.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <inttypes.h>
#include <stdio.h>

#include <Arduino.h>
#include <Helper.h>

#include <CarSpeed.h>

#include <ADC.h>
#include <DAC.h>
#include <PID_v1.h>

extern PID pid;
extern CarSpeed carSpeed;
extern ADC adc;
extern DAC dac;

void CarSpeed::re_init() { init(); }

void CarSpeed::init() {
  target_speed = 0;
  // pid = PID(&input_value, &output_setpoint, &target_speed, Kp, Ki, Kd, DIRECT);
  pid.SetMode(AUTOMATIC);
  printf("CarSpeed successfully initialized");
}

void CarSpeed::exit(void) { set_target_speed(0); }

void CarSpeed::set_target_speed(double speed) { target_speed = speed; }

double CarSpeed::get_target_speed() { return target_speed; }

double CarSpeed::get_current_speed() {
  // TODO: this function can be heavily optimized (i.e. use int instead of float, use less arithmetic operations

  // read analog value
  int16_t value = adc.read(ADC::Pin::MOTOR_SPEED);

  // convert value to voltage
  float voltage = value * 5.0 / 65536; // TODO: check if we really use 16bit ADS1115

  // convert value from voltage to revolutions per minute: 370rpm / V
  float wheel_circumference = 1.0; // TODO: set circumference = diameter*pi, in meters
  return (double)370 * wheel_circumference / 60.0;
}

void CarSpeed::update_pid(double Kp, double Ki, double Kd) { pid.SetTunings(Kp, Ki, Kd); }

void CarSpeed::task() {

  // polling loop
  while (1) {
    // update pid controller
    input_value = get_current_speed();
    pid.Compute();
    dac.set_pot(output_setpoint, DAC::pot_chan::POT_CHAN0); // TOOD: check that the value is in range

    // sleep
    vTaskDelay(sleep_polling_ms / portTICK_PERIOD_MS);
  }
}
