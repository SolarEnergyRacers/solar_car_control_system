//
// 16-Channel Pulse Width Modulation
//

#include "../../include/definitions.h"

#include <Wire.h>

#include "PWM.h"
#include <I2CBus.h>

#include <Adafruit_PWMServoDriver.h>

#define PWM_NUM_PORTS 16
#define PWM_MAX_VALUE 4096

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(I2C_ADDRESS_PCA9685, Wire);

void init_pwm(void) {

  // CRITICAL SECTION I2C: start
  xSemaphoreTake(i2c_mutex, portMAX_DELAY);

  // init device
  pwm.begin();

  // set up parameters
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(1600); // max pwm freq

  xSemaphoreGive(i2c_mutex);
  // CRITICAL SECTION I2C: end
}

void update_pwm(int channel, int value) {

  // check valid port
  if (channel < 0 || channel >= PWM_NUM_PORTS) {
    return;
  }
  // check valid value
  if (value < 0 || value > PWM_MAX_VALUE) {
    return;
  }

  // CRITICAL SECTION I2C: start
  xSemaphoreTake(i2c_mutex, portMAX_DELAY);

  pwm.setPWM(channel, 0, value);

  xSemaphoreGive(i2c_mutex);
  // CRITICAL SECTION I2C: end
}

void update_pwm_demo_task(void *pvParameter) {

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
