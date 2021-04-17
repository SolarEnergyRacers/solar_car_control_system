// (de-)activate functionality & devices
#ifndef LocalFunctionsAndDevices_H
#define LocalFunctionsAndDevices_H

#define ADC_ON true                         // analog digital converte, I2C (ADS1015, ADS1115)
#define BLINK_ON true                       // blink microcontroller LED 
#define COMMANDHANDLER_ON true              // command input and deploy
#define DAC_ON true                         // digital analog converter, I2C (DS1803)
#define DISPLAY_LARGE_INDICATOR_ON true     // indicator blink
#define DISPLAY_LARGE_ON true               // driver display, SPI (ILI9341)
#define DISPLAY_ON true                     // display elements
#define DS_ON true                          // 1-Wire Digital Thermometer (DS18B20)
#define GYRO_ACC_ON true                    // 6-axis Motion Tracking (BMI088)
#define INT_ON true                         // interrupt for I/O Expander
#define IOEXT_ON true                       // 8-Bit I/O Expander for I2C Bus (PCF8574)
#define PWM_ON true                         // 16-channel, 12-bit PWM Fm, I2C (PCA9685)
#define RTC_ON true                         // real time clock
#define SERIAL_ON true                      // write/read to/from console
#define SD_ON true                          // SD card
#define SIMULATOR_ON true

#endif // LocalFunctionsAndDevices_H
