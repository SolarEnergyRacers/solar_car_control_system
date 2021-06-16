//
// Indicators LEFT, RIGTH, WARN
//

#ifndef INDICATOR_H
#define INDICATOR_H

#include "DriverDisplayC.h"

// public
DriverDisplayC::INDICATOR getIndicator();
void setIndicator(DriverDisplayC::INDICATOR state);

// FreeRTOS - start
bool init_indicator(void);
void indicator_task(void *pvParameter);
// FreeRTOS - end

#endif // INDICATOR_H
