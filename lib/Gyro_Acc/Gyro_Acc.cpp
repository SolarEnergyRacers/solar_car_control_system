//
// 3-axis Gyroscope & 3-axis Accelerometer
//

#include <definitions.h>

#include <fmt/core.h>
#include <fmt/printf.h>
#include <iostream>
#include <stdio.h>
#include <string>

#include <BMI088.h> // gyro & acc lib
#include <Console.h>
#include <Gyro_Acc.h>
#include <I2CBus.h>

extern I2CBus i2cBus;
extern Console console;

void GyroAcc::re_init() { init(); }

void GyroAcc::init(void) {
  // check connection & report
  if (bmi088.isConnection()) {
    console << "[BMI088] is connected\n";

    // init sensor
    bmi088.initialize();
    console << "[BMI088] is initialized\n";

  } else {
    console << "[BMI088] is not connected\n";
  }
}

Float3D GyroAcc::read_gyroscope(void) {
  // allocate struct
  Float3D data;
  bmi088.getGyroscope(&data.x, &data.y, &data.z);
  return data;
}

Float3D GyroAcc::read_acceleration(void) {
  // allocate struct
  Float3D data;
  bmi088.getAcceleration(&data.x, &data.y, &data.z);
  return data;
}

Float3D gyro_last, acc_last;
void GyroAcc::task() {

  // define data structs
  Float3D gyro, acc;

  while (1) {

    // read the accel
    gyro = read_gyroscope();
    if (gyro_last.x != gyro.x || gyro_last.y != gyro.y || gyro_last.z != gyro.z) {
      gyro_last.x = gyro.x;
      gyro_last.y = gyro.y;
      gyro_last.z = gyro.z;
      //console << fmt::sprintf("[BMI088] gx=%f, gy=%f, gz=%f\n", gyro.x, gyro.y, gyro.z);
    }

    // read the gyro
    acc = read_acceleration();
    if (acc_last.x != acc.x || acc_last.y != acc.y || acc_last.z != acc.z) {
      acc_last.x = acc.x;
      acc_last.y = acc.y;
      acc_last.z = acc.z;
      //console << fmt::sprintf("[BMI088] ax=%f, ay=%f, az=%f\n", acc.x, acc.y, acc.z);
    }

    // sleep for 1s
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}