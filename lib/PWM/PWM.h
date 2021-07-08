//
// 16-Channel Pulse Width Modulation
//

#ifndef PWM_H
#define PWM_H

#include <Adafruit_PWMServoDriver.h>
#include <abstract_task.h>

class PWM : public abstract_task {
private:
    Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(I2C_ADDRESS_PCA9685, Wire);
public:
    string getName(void) { return "PWM"; };
    void init(void);
    void re_init(void);
    void exit(void);
    void task(void);
    void update_pwm(int channel, int value);
};

#endif // PWM_H
