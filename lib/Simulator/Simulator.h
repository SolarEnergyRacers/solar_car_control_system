//
// Simulator
//

#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#include <abstract_task.h>

class Simulator : public abstract_task {
private:
    // counters for demo display
    int counterIndicator = 0;
    int counterSpeed = 0;
    int counterPV = 0;
public:
    string getName(void) { return "Simulator"; };
    void init(void);
    void re_init(void);
    void exit(void);
    void task(void);
};

#endif // SIMULATOR_H