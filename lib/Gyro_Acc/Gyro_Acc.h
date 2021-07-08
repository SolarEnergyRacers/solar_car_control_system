//
// 3-axis Gyroscope & 3-axis Accelerometer
//

#ifndef GYRO_ACC_H
#define GYRO_ACC_H

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

#endif // GYRO_ACC_H
