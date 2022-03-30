//
// 3-axis Gyroscope & 3-axis Accelerometer
//

#ifndef SOLAR_CAR_CONTROL_SYSTEM_GYRO_ACC_H
#define SOLAR_CAR_CONTROL_SYSTEM_GYRO_ACC_H

#include <abstract_task.h>

typedef struct {
  float x, y, z;
} Float3D;

//void read_gyro_acc_demo_task(void *pvParameter); // TODO: can eventually be removed

class GyroAcc : public abstract_task {
private:
public:
  string getName(void) { return "Gyro"; };
  void init();
  void re_init();
  void task(void);
  Float3D read_gyroscope(void);
  Float3D read_acceleration(void);
};

#endif // SOLAR_CAR_CONTROL_SYSTEM_GYRO_ACC_H
