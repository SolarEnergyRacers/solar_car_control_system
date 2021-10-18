//
// 3-axis Gyroscope & 3-axis Accelerometer
//

#ifndef SOLAR_CAR_CONTROL_SYSTEM_GYRO_ACC_H
#define SOLAR_CAR_CONTROL_SYSTEM_GYRO_ACC_H

typedef struct {
  float x, y, z;
} Float3D;

void read_gyro_acc_demo_task(void *pvParameter); // TODO: can eventually be removed

class GyroAcc {
private:
public:
  void init();
  void re_init();
  Float3D read_gyroscope(void);
  Float3D read_acceleration(void);
};

#endif // SOLAR_CAR_CONTROL_SYSTEM_GYRO_ACC_H
