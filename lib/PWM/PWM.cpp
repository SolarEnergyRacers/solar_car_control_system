//
// 16-Channel Pulse Width Modulation
//

#include <definitions.h>

#include <Wire.h>

#include "PWM.h"
#include <I2CBus.h>

#include <Adafruit_PWMServoDriver.h>

extern I2CBus i2cBus;

void PWM::init(void) {

  // CRITICAL SECTION I2C: start
  xSemaphoreTake(i2cBus.mutex, portMAX_DELAY);

  // init device
  pwm.begin();

  // set up parameters
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(1600); // max pwm freq

  xSemaphoreGive(i2cBus.mutex);
  // CRITICAL SECTION I2C: end
}

void PWM::re_init() {
    init();
}

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
  xSemaphoreTake(i2cBus.mutex, portMAX_DELAY);

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
      printf("[PCA9685] set all outputs to: %d\n", i);

      // sleep for 1s
      vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
  }
}
