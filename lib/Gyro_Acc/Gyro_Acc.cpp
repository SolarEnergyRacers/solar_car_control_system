//
// 3-axis Gyroscope & 3-axis Accelerometer
//

#include <definitions.h>

#include "BMI088.h" // gyro & acc lib
#include <I2CBus.h>

#include "Gyro_Acc.h"

extern I2CBus i2cBus;

void GyroAcc::re_init() {
    init();
}

void GyroAcc::init(void) {
  // CRITICAL SECTION I2C: start
  xSemaphoreTake(i2cBus.mutex, portMAX_DELAY);

  // check connection & report
  if (bmi088.isConnection()) {
    printf("[BMI088] is connected\n");

    // init sensor
    bmi088.initialize();
    printf("[BMI088] is initialized\n");

  } else {
    printf("[BMI088] is not connected\n");
  }

  xSemaphoreGive(i2cBus.mutex);
  // CRITICAL SECTION I2C: end
}

Float3D GyroAcc::read_gyroscope(void) {

  // allocate struct
  Float3D data;

  // CRITICAL SECTION I2C: start
  xSemaphoreTake(i2cBus.mutex, portMAX_DELAY);

  // read the gyro
  bmi088.getGyroscope(&data.x, &data.y, &data.z);

  xSemaphoreGive(i2cBus.mutex);
  // CRITICAL SECTION I2C: end

  return data;
}

Float3D GyroAcc::read_acceleration(void) {

  // allocate struct
  Float3D data;

  // CRITICAL SECTION I2C: start
  xSemaphoreTake(i2cBus.mutex, portMAX_DELAY);

  // read the gyro
  bmi088.getAcceleration(&data.x, &data.y, &data.z);

  xSemaphoreGive(i2cBus.mutex);
  // CRITICAL SECTION I2C: end

  return data;
}

extern GyroAcc gyroAcc;
void read_gyro_acc_demo_task(void *pvParameter) {

  // define data structs
  Float3D gyro, acc;

  while (1) {

    // read the accel
    gyro = gyroAcc.read_gyroscope();

    // read the gyro
    acc = gyroAcc.read_acceleration();

    // print result
    printf("[BMI088] ax=%f, ay=%f, az=%f\n", acc.x, acc.y, acc.z);
    printf("[BMI088] gx=%f, gy=%f, gz=%f\n", gyro.x, gyro.y, gyro.z);

    // sleep for 1s
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}