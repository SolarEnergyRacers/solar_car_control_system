//
// 3-axis Gyroscope & 3-axis Accelerometer
//

#ifndef GYRO_ACC_H
#define GYRO_ACC_H

typedef struct {
  float x, y, z;
} Float3D;

void init_gyro_acc(void);

Float3D read_gyroscope(void);
Float3D read_acceleration(void);

void read_gyro_acc_demo_task(void *pvParameter);

#endif // GYRO_ACC_H
