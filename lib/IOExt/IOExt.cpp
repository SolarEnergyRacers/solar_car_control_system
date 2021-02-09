//
// PCF8574 I/O Extension over I2C  !!! UNTESTED !!!
//

#include "../../include/definitions.h"

#include <I2CBus.h>
#include "IOExt.h"

#include <Wire.h> // I2C
#include "PCF8574.h" // PCF8574


#define PCF8574_NUM_PORTS 8
void key_pressed_interrupt_handler();

PCF8574 pcf8574(I2C_ADDRESS_PCF8574, PCF8574_INTERRUPT_PIN, key_pressed_interrupt_handler);


void init_ioext(){

    // setup pins
    pcf8574.pinMode(P0, INPUT_PULLUP);
    pcf8574.pinMode(P1, INPUT_PULLUP);
    pcf8574.pinMode(P2, INPUT);
    pcf8574.pinMode(P3, INPUT);
    pcf8574.pinMode(P4, OUTPUT);
    pcf8574.pinMode(P5, OUTPUT, HIGH);
    pcf8574.pinMode(P6, OUTPUT);
    pcf8574.pinMode(P7, OUTPUT, LOW);

    // start
    if (pcf8574.begin()){
        printf("[PCF8574] Init successful..\n");
    } else {
        printf("[PCF8574] Init failed..\n");
    }

}

void set_ioext(int port, bool value){

    // check port
    if(port < 0 || port >= PCF8574_NUM_PORTS){
        return;
    }

    // CRITICAL SECTION I2C: start
    xSemaphoreTake(i2c_mutex, portMAX_DELAY);

    pcf8574.digitalWrite(port, value);

    xSemaphoreGive(i2c_mutex);
    // CRITICAL SECTION I2C: end
}

int get_ioext(int port){

    // check port
    if(port < 0 || port >= PCF8574_NUM_PORTS){
        return -1;
    }

    // CRITICAL SECTION I2C: start
    xSemaphoreTake(i2c_mutex, portMAX_DELAY);

    int value = pcf8574.digitalRead(P0);

    xSemaphoreGive(i2c_mutex);
    // CRITICAL SECTION I2C: end

    return value;
}

void io_ext_demo_task(void *pvParameter){

    // polling loop
    while(1){

        // blink port 6
        set_ioext(6, !get_ioext(6));

        // sleep for 1s
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}