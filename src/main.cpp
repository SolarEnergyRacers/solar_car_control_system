/*
 *  SER - Solar Car Control System - Main Routine for setting up tasks,
 * initialize devices, ..
 *
 * clang style:
 *    find . -name "*.cpp" -o -name "*.c" -o -name "*.h" | grep '\./lib' | xargs
 * -I {} clang-format -i {}
 */

// standard libraries
#include <stdio.h>

// FreeRTOS / Arduino
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

// project variables
#include <sdkconfig.h>

// local definitions
#include <definitions.h>

// local libs
#include <ADC.h>
#include <CANBus.h>
#include <CmdHandler.h>
#include <DAC.h>
#include <Display.h>
#include <DriverDisplayC.h>
#include <Gyro_Acc.h>
#include <I2CBus.h>
#include <IOExt.h>
#include <Indicator.h>
#include <OneWireBus.h>
#include <PWM.h>
#include <RTC.h>
#include <SDCard.h>
#include <SPIBus.h>
#include <Serial.h>
#include <Simulator.h>
#include <Temp.h>
#include <GPIO.h>
#include <string>
#include <system.h>

#include <LocalFunctionsAndDevices.h>
#include <abstract_task.h>

// add C linkage definition
extern "C" {
void app_main(void);
}

using namespace std;
// using namespace DriverDisplayC;

#if ADC_ON
ADC adc;
#endif

//#if CAN_ON // TODO: gets a linking-error if we set CAN_ON to true
CanBus can;
//#endif

OneWireBus oneWireBus;
SPIBus spiBus;
I2CBus i2cBus;

//#if DS_ON
Temp ds;
//#endif

//#if SD_ON
SDCard sdCard;
//#endif

//#if COMMANDHANDLER_ON
CmdHandler cmdHandler;
//#endif

//#if DAC_ON
DAC dac;
//#endif

//#if SERIAL_ON
Uart uart;
//#endif

//#if GYRO_ACC_ON
GyroAcc gyroAcc;
//#endif

//#if DISPLAY_LARGE_INDICATOR_ON
Indicator indicator;
//#endif

//#if IOEXT_ON
IOExt ioExt;
//#endif

//#if PWM_ON
PWM pwm;
//#endif

//#if DISPLAY_ON
Display disp;
//#endif

//#if RTC_ON
RTC rtc;
//#endif

//#if INT_ON
GPInputOutput gpio;
//#endif

//#if SIMULATOR_ON
Simulator simulator;
//#endif

void app_main(void) {
    bool startOk = true;

    // init arduino library
    initArduino();

    // init serial output for  console

    Serial.begin(115200);
    delay(2000);
    printf("\n--------------------\n");
    Serial.println("esp32dev + free RTOS\n");
    printf("--------------------\n");

    // report chip info
    chip_info();

    // init buses
    oneWireBus.init();
    i2cBus.init();
    spiBus.init();

    i2cBus.scan_i2c_devices();

    // ---- init modules ----
    if (BLINK_ON) {
    }
    if (DISPLAY_LARGE_ON) {
        DriverDisplayC::instance()->init();
    }
    if (DISPLAY_LARGE_INDICATOR_ON) {
        // startOk &= init_indicator(); // TODO: restore this functionality
        indicator.init();
    }
    if (COMMANDHANDLER_ON) {
        cmdHandler.init();
    }
    if (ADC_ON) {
        adc.init();
        // example: printf("Motor speed is: %d\n", adc.read(ADC::Pin::MOTOR_SPEED));
    }
    if (DS_ON) {
        ds.init();
    }
    if (GYRO_ACC_ON) {
        gyroAcc.init();
    }
    if (PWM_ON) {
        pwm.init();
    }
    if (RTC_ON) {
        rtc.init();
    }
    if (SD_ON) {
        sdCard.init();
    }
    if (INT_ON) {
        gpio.init();
        gpio.register_gpio_interrupt();
    }
    if (DISPLAY_ON) {
        disp.init();
    }
    if (IOEXT_ON) {
        ioExt.init();
    }
    if (DAC_ON) {
        dac.init();
    }
    if (SERIAL_ON) {
        uart.init();
    }
    if (SIMULATOR_ON) {
        simulator.init();
    }
    if (CAN_ON) {
        can.init();
    }

    if (!startOk) {
        printf("ERROR in init sequence(s). System halted!\n");
        exit(0);
    }
    printf("-----------------------------------------------------------------\n");
    printf("Startup sequence(s) successful. System creating FreeRTOS tasks...\n");
    printf("-----------------------------------------------------------------\n");

    // ---- create tasks ----
    if (DISPLAY_ON) {
        printf(" - draw_display_demo_task\n");
        disp.create_task();
    }
    if (DISPLAY_LARGE_ON) {
        DriverDisplayC::instance()->create_task();
        printf("done\n");
    }

    if (DISPLAY_LARGE_INDICATOR_ON) {
        printf(" - indicator_task\n");
        indicator.create_task();
    }
//    if (BLINK_ON) { // not activated
//        printf(" - blink_demo_task\n");
//        xTaskCreate(&blink_demo_task, "blink_demo_task", CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
//    }
    if (ADC_ON) {
        xTaskCreate(&read_adc_acceleration_recuperation, "read_adc_task",
                    CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
    }
    if (DS_ON) {
        printf(" - read_ds_demo_task\n");
        xTaskCreate(&read_ds_demo_task, "read_ds_demo_task", CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
    }
    if (GYRO_ACC_ON) {
        printf(" - read_gyro_acc_demo_task\n");
        xTaskCreate(&read_gyro_acc_demo_task, "read_gyro_acc_demo_task", CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL,
                    5, NULL);
    }
    if (PWM_ON) {
        printf(" - update_pwm_demo_task\n");
        pwm.create_task();
    }
    if (RTC_ON) {
        printf(" - read_rtc_demo_task\n");
        rtc.create_task();
    }
    if (SD_ON) {
        printf(" - write_sdcard_demo_task\n");
        xTaskCreate(&write_sdcard_demo_task, "write_sdcard_demo_task", CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5,
                    NULL);
    }
    if (INT_ON) {
        printf(" - int_report_demo_task\n");
        gpio.create_task();
    }
    if (SIMULATOR_ON) {
        printf(" - simulator_task\n");
        simulator.create_task();
    }
    if (IOEXT_ON) {
        printf(" - IOExt2_task\n");
        ioExt.create_task();
    }
    if (DAC_ON) {
        printf(" - DAC DAC DAC\n");
    }
    if (COMMANDHANDLER_ON) {
        printf(" - command_handler_task\n");
        cmdHandler.create_task();
    }
    if (SERIAL_ON) {
        printf(" - serial_demo_task\n");
        //xTaskCreate(&serial_demo_task, "serial_demo_task", CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
    }
    if (CAN_ON) {
        printf(" - read_can_demo_task\n");
        xTaskCreate(&read_can_demo_task, "can_task", CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
    }
    printf("-----------------------------------------------------------------\n");
    printf("Creating FreeRTOS tasks successful. System running.\n");
    printf("-----------------------------------------------------------------\n\n");
    // if (DISPLAY_LARGE_ON) {
    //   DriverDisplayC::instance()->driver_display_demo_screen();
    // }
}
