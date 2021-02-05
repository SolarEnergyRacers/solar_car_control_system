/*
 *  TODO: add description & license
 */

// standard library includes
#include <stdio.h>

// FreeRTOS / Arduino
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h> // semaphore

#include <Arduino.h>

// esp32 includes
#include <driver/gpio.h>
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "sdkconfig.h"

// local includes
#include "definitions.h"

// external libs
#include <OneWire.h> // 1-Wire
#include <Wire.h> // I2C
#include <SPI.h> // SPI

// local libs
#include<I2C.h>
#include <ADC.h>// analog to digital converter

#include <DallasTemperature.h> // DS18B20
OneWire oneWire(ONEWIRE_PIN);
DallasTemperature ds(&oneWire);

#include "BMI088.h" // gyro & acc

#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, Wire);


#include <RtcDS1307.h>
RtcDS1307 <TwoWire> Rtc(Wire);


#include <SD.h> // sd card

#include <Adafruit_GFX.h> // graphics library
#include <Adafruit_SSD1305.h> // display
#define OLED_RESET 9
Adafruit_SSD1305 display(128, 64, &Wire, OLED_RESET);

// add C linkage definition
extern "C" {
    void app_main(void);
}

void blink_task(void *pvParameter) {
    /* Configure the IOMUX register for pad BLINK_GPIO (some pads are
       muxed to GPIO on reset already, but some default to other
       functions and need to be switched to GPIO. Consult the
       Technical Reference for a list of pads and their default
       functions.)
    */
    gpio_pad_select_gpio(LED_BUILTIN);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(LED_BUILTIN, GPIO_MODE_OUTPUT);
    while (1) {
        /* Blink off (output low) */
        gpio_set_level(LED_BUILTIN, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        /* Blink on (output high) */
        gpio_set_level(LED_BUILTIN, 1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void chip_info(void *pvParameter) {

    // print chip information
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is ESP32 chip with %d CPU cores, WiFi%s%s, ",
           chip_info.cores,
           (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
           (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    printf("silicon revision %d, ", chip_info.revision);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
           (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");
}

void restart(void *pvParameter) {

    // count time down
    for (int i = 10; i >= 0; i--) {
        printf("Restarting in %d seconds...\n", i);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    // actual restart
    printf("Restarting now.\n");
    fflush(stdout);
    esp_restart();
}

void init_onewire(void) {}



void init_ds() {

    ds.begin();

    printf("[OneWire] num devices on bus: %d\n", ds.getDeviceCount());


    DeviceAddress device_addr;

    oneWire.reset_search();
    while (oneWire.search(device_addr)) {

        printf("[DS18B20] Address:");
        for (uint8_t i = 0; i < 8; i++) {

            printf(" %d", device_addr[i]);
        }
        printf(" ");

        printf("Resolution: %d", ds.getResolution(device_addr));
        printf(" ");

        printf("Power Mode: ");
        if (ds.isParasitePowerMode()) {
            printf("External");
        } else {
            printf("Parasite");
        }
        printf("\n");
    }
}

void read_ds(void *pvParameter) {

    // polling loop
    while (1) {

        // request all temperature sensor readings
        ds.requestTemperatures();

        // print all results
        for (int i = 0; i < ds.getDeviceCount(); i++) {
            printf("[DS18B20] Temperature: %fC / %fF\n", ds.getTempCByIndex(i), ds.getTempFByIndex(i));
        }

        // sleep for 1s
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void init_gyro_acc(void) {

    // start the sensors
    if (bmi088.isConnection()) {
        bmi088.initialize();
        printf("[BMI088] is connected\n");
    } else {
        printf("[BMI088] is not connected\n");
    }

}

void read_gyro_acc(void *pvParameter) {

    float ax = 0, ay = 0, az = 0;
    float gx = 0, gy = 0, gz = 0;

    while (1) {

        // read the accel
        bmi088.getAcceleration(&ax, &ay, &az);
        // read the gyro
        bmi088.getGyroscope(&gx, &gy, &gz);

        // print result
        printf("[BMI088] ax=%f, ay=%f, az=%f\n", ax, ay, az);
        printf("[BMI088] gx=%f, gy=%f, gz=%f\n", gx, gy, gz);

        // sleep for 1s
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void init_pwm(void) {
    pwm.begin();
    pwm.setOscillatorFrequency(27000000);
    pwm.setPWMFreq(1600);  // max pwm frequency
}

void update_pwm(void *pvParameter) {

    while (1) {

        for (int i = 0; i < 4096; i += 128) {
            for (int output = 0; output < 16; output++) {
                pwm.setPWM(output, 0, i);
            }
            printf("[PCA9685] value=%d\n", i);

            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
    }
}

void init_rtc(void) {

    Rtc.Begin();

    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
    printf("[DS1307] %02u/%02u/%04u %02u:%02u:%02u\n", compiled.Month(), compiled.Day(), compiled.Year(), compiled.Hour(), compiled.Minute(), compiled.Second());

    if (!Rtc.IsDateTimeValid()) {
        if (Rtc.LastError() != 0) {
            // we have a communications error
            printf("RTC communications error = %d\n", Rtc.LastError());
        } else {
            // Common Causes:
            //    1) first time you ran and the device wasn't running yet
            //    2) the battery on the device is low or even missing

            printf("RTC lost confidence in the DateTime! Set datetime to compile time of this binary.\n");
            // following line sets the RTC to the date & time this sketch was compiled
            // it will also reset the valid flag internally unless the Rtc device is
            // having an issue

            Rtc.SetDateTime(compiled);
        }
    }

    if (!Rtc.GetIsRunning()) {
        printf("RTC was not actively running, starting now\n");
        Rtc.SetIsRunning(true);
    }

    RtcDateTime now = Rtc.GetDateTime();
    if (now < compiled) {
        printf("RTC is older than compile time!  (Updating DateTime)\n");
        Rtc.SetDateTime(compiled);
    } else if (now > compiled) {
        printf("RTC is newer than compile time. (this is expected)\n");
    } else if (now == compiled) {
        printf("RTC is the same as compile time! (not expected but all is fine)\n");
    }

    // never assume the Rtc was last configured by you, so
    // just clear them to your needed state
    Rtc.SetSquareWavePin(DS1307SquareWaveOut_Low);
}


void read_rtc(void *pvParameter) {

    while (1) {
        if (!Rtc.IsDateTimeValid()) {
            if (Rtc.LastError() != 0) {
                // we have a communications error
                // see https://www.arduino.cc/en/Reference/WireEndTransmission for
                // what the number means
                printf("RTC communications error = %d\n", Rtc.LastError());
            } else {
                // Common Causes:
                //    1) the battery on the device is low or even missing and the power line was disconnected
                printf("RTC lost confidence in the DateTime!\n");
            }
        }

        RtcDateTime now = Rtc.GetDateTime();

        printf("%02u/%02u/%04u %02u:%02u:%02u\n", now.Month(), now.Day(), now.Year(), now.Hour(), now.Minute(), now.Second());


        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

File dataFile;
void init_sdcard(void){

    if (!SD.begin(SPI_CS_SDCARD)) {
        printf("[SDCard] Initialization failed\n");
    } else {
        printf("[SDCard] Initialization successful\n");

        dataFile = SD.open("/datalog.txt", FILE_APPEND); //FILE_WRITE);

    }
}

void write_sdcard(void *pvParameter) {


    int counter = 0;

    while(1){

        if (dataFile) {
            dataFile.print(counter);
            dataFile.println("");
            printf("[SDCard] Write to sdcard: %d\n", counter++);
        } else {
            printf("[SDCard] Error opening file.\n");
        }
        // dataFile.close();
        dataFile.flush(); // ensure write-back

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
volatile int interrupt_counter = 0;
void IRAM_ATTR handle_gpio_interrupt();
void register_gpio_interrupt(){

    printf("[HW Interrupt] Register gpio interrupt pin %d (falling edge)\n", GPIO_INTERRUPT_PIN);
    pinMode(GPIO_INTERRUPT_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(GPIO_INTERRUPT_PIN), handle_gpio_interrupt, FALLING);

}

void IRAM_ATTR handle_gpio_interrupt() {
    portENTER_CRITICAL_ISR(&mux);
    interrupt_counter++;
    portEXIT_CRITICAL_ISR(&mux);
}

void int_report(void *pvParameter){

    while(1){

        printf("[INT] Number of interrupts: %d\n", interrupt_counter);

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void init_display(void){

    if (!display.begin(0x3C) ) {
        printf("[Display] Unable to initialize OLED screen.\n");
    } else {
        printf("[Display] Screen initialize successfully.\n");
    }

    // init done
    display.display(); // show splashscreen
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    display.clearDisplay();   // clears the screen and buffer



}


void draw_display(void *pvParameter){

    while(1){

        display.clearDisplay();   // clears the screen and buffer

        display.setTextSize(1);
        display.setTextWrap(false);
        display.setTextColor(WHITE);
        display.setCursor(0,0);

        for (uint8_t i=0; i < 168; i++) {
            if (i == '\n') continue;
            display.write(i);
            if ((i > 0) && (i % 21 == 0))
                display.println();
        }
        display.display();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

#define BLINK_ON true
#define ADC_ON true
#define DS_ON false
#define GYRO_ACC_ON false
#define PWM_ON false
#define RTC_ON false
#define INT_ON false
#define SD_ON false
#define DISPLAY_ON true


void app_main(void) {

    // init arduino library
    initArduino();

    // report chip info
    chip_info(NULL);

    // init buses
    init_i2c();
    init_onewire();

    // init modules & create tasks
    if(BLINK_ON){
        xTaskCreate(&blink_task, "blink_task", configMINIMAL_STACK_SIZE, NULL, 5, NULL);
    }
    if(ADC_ON){
        init_adc();
        xTaskCreate(&read_adc_demo_task, "read_adc_task", 2500, NULL, 5, NULL);
    }
    if(DS_ON) {
        init_ds();
        xTaskCreate(&read_ds, "read_ds_task", 2500, NULL, 5, NULL);
    }
    if(GYRO_ACC_ON){
        init_gyro_acc();
        xTaskCreate(&read_gyro_acc, "read_gyro_acc_task", 2500, NULL, 5, NULL);
    }
    if(PWM_ON) {
        init_pwm();
        xTaskCreate(&update_pwm, "update_pwm_task", 2500, NULL, 5, NULL);
    }
    if(RTC_ON) {
        init_rtc();
        xTaskCreate(&read_rtc, "read_adc_task", 2500, NULL, 5, NULL);
    }
    if(SD_ON){
        init_sdcard();
        xTaskCreate(&write_sdcard, "write_sdcard_task", 2500, NULL, 5, NULL);
    }
    if(INT_ON) {
        // register interrupt handler
        register_gpio_interrupt();
        xTaskCreate(&int_report, "int_report_task", 2500, NULL, 5, NULL);
    }
    if(DISPLAY_ON){
        init_display();
        xTaskCreate(&draw_display, "display_task", 2500, NULL, 5, NULL);
    }
}
