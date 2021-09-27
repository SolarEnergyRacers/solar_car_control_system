//
// Car Speed PID Control
//

#ifndef SOLAR_CAR_CONTROL_SYSTEM_CARSPEED_H
#define SOLAR_CAR_CONTROL_SYSTEM_CARSPEED_H

class CarSpeed {
private:
    uint32_t sleep_polling_ms = 200;
    float target_speed;
    double target_speed, input_value, output_setpoint;
    double Kp=2, Ki=5, Kd=1;
    PID pid(&input_value, &output_setpoint, &target_speed, Kp, Ki, Kd, DIRECT);
public:
    string getName(void) { return "CarSpeed"; };
    void init(void);
    void re_init(void);
    void exit(void);
    void task(void);
    void set_target_speed();
    void get_target_speed();
    void get_current_speed(); // speed in m/s
    void update_pid(double Kp, double Ki, double Kd);
};

#endif //SOLAR_CAR_CONTROL_SYSTEM_CARSPEED_H
