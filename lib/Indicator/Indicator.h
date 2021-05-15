//
// Indicators LEFT, RIGTH, WARN
//

#ifndef INDICATOR_H
#define INDICATOR_H

// public
INDICATOR getIndicator();
void setIndicator(INDICATOR state);

// FreeRTOS - start
bool init_indicator(void);
void indicator_task(void *pvParameter);
// FreeRTOS - end

#endif // INDICATOR_H
