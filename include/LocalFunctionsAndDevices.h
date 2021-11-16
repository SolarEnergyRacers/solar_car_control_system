// (de-)activate functionality & devices
#ifndef LocalFunctionsAndDevices_H
#define LocalFunctionsAndDevices_H

#define ADC_ON true              // analog digital converte, I2C (ADS1015, ADS1115)
#define CAN_ON false             // CAN bus (MPPT, BMS, ..)
#define CARCONTROL_ON true       // central car control task
#define CARSPEED_ON false        // car pid speed controller
#define COMMANDHANDLER_ON true   // command input and deploy
#define DAC_ON false             // digital analog converter, I2C (DS1803)
#define DRIVER_DISPLAY_ON true   // driver display, SPI (ILI9341)
#define DS_ON false              // 1-Wire Digital Thermometer (DS18B20)
#define ENGINEER_DISPLAY_ON true // display elements
#define GYRO_ACC_ON false        // 6-axis Motion Tracking (BMI088)
#define INDICATOR_ON true        // indicator blink
#define INT_ON false             // interrupt for I/O Expander
#define IOEXT_ON false            // 8-Bit I/O Expander for I2C Bus (PCF8574)
#define IOEXT2_ON true            // 8-Bit I/O Expander for I2C Bus (MCP23017)
#define LIFESIGN_ON false        // LifeSign on Screens
#define PWM_ON false             // 16-channel, 12-bit PWM Fm, I2C (PCA9685)
#define RTC_ON true              // real time clock, I2C
#define SD_ON false              // SD card
#define SERIAL_ON false          // write/read to/from console

#define DEBUGLEVEL_VERBOSED true

#define WithTaskSuspend false // use of FreeRTOS task suspend functionality

#endif // LocalFunctionsAndDevices_H
