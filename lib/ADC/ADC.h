//
// Analog to Digital Converter
//

#ifndef ADC_H
#define ADC_H

void init_adc();
int16_t read_adc(int port);
void read_adc_demo_task(void *pvParameter);
void read_adc_acceleration_recuperation(void *pvParameter);

#endif // ADC_H
