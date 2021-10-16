// (de-)activate functionality & devices
#ifndef LocalFunctionsAndDevices_H
#define LocalFunctionsAndDevices_H

#define ADC_ON true                     // analog digital converte, I2C (ADS1015, ADS1115)
#define BLINK_ON true                   // blink microcontroller LED
#define COMMANDHANDLER_ON true          // command input and deploy
#define DAC_ON false                    // digital analog converter, I2C (DS1803)
#define DISPLAY_LARGE_INDICATOR_ON true // indicator blink
#define DRIVER_DISPLAY_ON true           // driver display, SPI (ILI9341)
#define ENGINEER_DISPLAY_ON false                // display elements
#define DS_ON false                     // 1-Wire Digital Thermometer (DS18B20)
#define GYRO_ACC_ON false               // 6-axis Motion Tracking (BMI088)
#define INT_ON false                    // interrupt for I/O Expander
#define IOEXT_ON true                   // 8-Bit I/O Expander for I2C Bus (PCF8574)
#define PWM_ON false                    // 16-channel, 12-bit PWM Fm, I2C (PCA9685)
#define RTC_ON false                    // real time clock, I2C
#define SERIAL_ON false                 // write/read to/from console
#define SD_ON false                     // SD card
#define CAN_ON false                    // CAN bus (MPPT, BMS, ..)
#define CARSPEED_ON false               // car pid speed controller
#define DEBUGLEVEL_VERBOSED true

#endif // LocalFunctionsAndDevices_H
