// (de-)activate functionality & devices
#ifndef LocalFunctionsAndDevices_H
#define LocalFunctionsAndDevices_H

#define ADC_ON true              // analog digital converte, I2C (ADS1015, ADS1115)
#define CAN_ON true              // CAN bus (MPPT, BMS, ..)
#define CARCONTROL_ON true       // central car control task
#define CARSPEED_ON true         // car pid speed controller
#define COMMANDHANDLER_ON true   // command input and deploy
#define DAC_ON true              // digital analog converter, I2C (DS1803)
#define DRIVER_DISPLAY_ON true   // driver display, SPI (ILI9341)
#define DS_ON false              // 1-Wire Digital Thermometer (DS18B20)
#define ENGINEER_DISPLAY_ON true // display elements
#define GYRO_ACC_ON false        // 6-axis Motion Tracking (BMI088)
#define INDICATOR_ON true        // indicator blink
#define INT_ON false             // interrupt for I/O Expander
#define IOEXT2_ON true           // 8-Bit I/O Expander for I2C Bus (MCP23017)
#define LIFESIGN_ON true         // LifeSign on Screens
#define PWM_ON false             // 16-channel, 12-bit PWM Fm, I2C (PCA9685)
#define RTC_ON true              // real time clock, I2C
#define SD_ON true               // SD card
#define SERIAL_RADIO_ON true     // additionally write/read to/from the Serial2, the radio console

#define VCC_12V true             // the break relais only works with 12V. If you want without external power supply that means with only 5V the logic must be inverted.

#define DEBUGLEVEL_VERBOSED true

#define WithTaskSuspend false // use of FreeRTOS task suspend functionality

#endif // LocalFunctionsAndDevices_H
