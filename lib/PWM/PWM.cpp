//
// 16-Channel Pulse Width Modulation
//

#include <definitions.h>

// standard libraries
#include <fmt/core.h>
#include <iostream>
#include <stdio.h>
#include <string>

#include <Wire.h>

#include <Console.h>
#include <Helper.h>
#include <I2CBus.h>
#include <PWM.h>

#include <Adafruit_PWMServoDriver.h>

extern Console console;
extern I2CBus i2cBus;

string PWM::init(void) {
  bool hasError = false;
  xSemaphoreTakeT(i2cBus.mutex);

  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(1600); // max pwm freq

  xSemaphoreGive(i2cBus.mutex);

  return fmt::format("[{}] PWM initialized.", hasError ? "--" : "ok");
}

string PWM::re_init() { return init(); }

void PWM::exit() {
  // TODO
}

void PWM::update_pwm(int channel, int value) {

  // check valid port
  if (channel < 0 || channel >= PWM_NUM_PORTS) {
    return;
  }
  // check valid value
  if (value < 0 || value > PWM_MAX_VALUE) {
    return;
  }

  // CRITICAL SECTION I2C: start
  xSemaphoreTakeT(i2cBus.mutex);

  pwm.setPWM(channel, 0, value);

  xSemaphoreGive(i2cBus.mutex);
  // CRITICAL SECTION I2C: end
}

void PWM::task() {

  while (1) {

    // update all channels
    for (int i = 0; i < 4096; i += 128) {
      for (int output = 0; output < 16; output++) {
        update_pwm(output, i);
      }
      // report current value
      console << "[PCA9685] set all outputs to: " << i;

      // sleep for 1s
      vTaskDelay(sleep_polling_ms / portTICK_PERIOD_MS);
    }
  }
}
