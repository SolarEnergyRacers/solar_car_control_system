//
// Indicators LEFT, RIGTH, WARN
//

#ifndef INDICATOR_H
#define INDICATOR_H

#include "DriverDisplayC.h"

class Indicator : public abstract_task {
private:
    volatile bool blinkState = false;
    volatile INDICATOR curState = INDICATOR::OFF;
public:
    string getName(void);
    void init(void);
    void re_init(void);
    void exit(void);
    void task(void); // this is the actual task
    INDICATOR getIndicator();
    void setIndicator(INDICATOR state);
};

#endif // INDICATOR_H
