//
// PCF8574 I/O Extension over I2C  !!! UNTESTED !!!
//

#include "../../include/definitions.h"
#include <I2CBus.h>
#include <Wire.h>    // I2C
#include "PCF8574.h" // PCF8574

#include "DriverDisplay.h"
#include "IOExt.h"
#include "Indicator.h"

#define PCF8574_NUM_PORTS 8

PCF8574 pcf8574(I2C_ADDRESS_PCF8574, I2C_SDA, I2C_SCL, PCF8574_INTERRUPT_PIN, key_pressed_interrupt_handler);

int speed = 0;
int taskSleep = 10;

void scan()
{
    /* I2C slave Address Scanner
    for 5V bus
        * Connect a 4.7k resistor between SDA and Vcc
        * Connect a 4.7k resistor between SCL and Vcc
    for 3.3V bus
        * Connect a 2.4k resistor between SDA and Vcc
        * Connect a 2.4k resistor between SCL and Vcc
    */
    Serial.println("Scanning I2C Addresses");
    uint8_t cnt = 0;
    for (uint8_t i = 0; i < 128; i++)
    {
        Wire.beginTransmission(i);
        uint8_t ec = Wire.endTransmission(true);
        if (ec == 0)
        {
            if (i < 16)
                Serial.print('0');
            Serial.print(i, HEX);
            cnt++;
        }
        else
        {
            Serial.print("--");
        }
        Serial.print(' ');
        if ((i & 0x0f) == 0x0f)
        {
            Serial.println();
        }
    }
    Serial.printf("Scan Completed, %d I2C Devices found.\n", cnt);
}

bool i2cReady(uint8_t adr)
{
    uint32_t timeout = millis();
    bool ready = false;
    while ((millis() - timeout < 100) && (!ready))
    {
        Wire.beginTransmission(adr);
        ready = (Wire.endTransmission() == 0);
    }
    return ready;
}

void init_ioext()
{

    // CRITICAL SECTION I2C: start
    xSemaphoreTake(i2c_mutex, portMAX_DELAY);

    // setup pins
    pcf8574.pinMode(P0, OUTPUT, LOW);
    pcf8574.pinMode(P1, INPUT);
    pcf8574.pinMode(P2, INPUT);
    pcf8574.pinMode(P3, INPUT);
    pcf8574.pinMode(P4, INPUT);
    pcf8574.pinMode(P5, INPUT);
    pcf8574.pinMode(P6, INPUT);
    pcf8574.pinMode(P7, INPUT);

    Wire.begin();
    scan();

    // start
    if (pcf8574.begin())
    {
        printf("[PCF8574] Init successful.\n");
    }
    else
    {
        printf("[PCF8574] Init failed.\n");
    }

    xSemaphoreGive(i2c_mutex);
    // CRITICAL SECTION I2C: end
}

//volatile PCF8574::DigitalInput digitalIn;
volatile bool ioInterruptRequest = false;
void key_pressed_interrupt_handler()
{
    ioInterruptRequest = true;
    //digitalIn = pcf8574.digitalReadAll();
    //buf = readBuffer();
    // Wire.requestFrom(0x20,(uint8_t)1);
    // ioBuf = Wire.read();
}

void _speedCheck(int speed)
{
    if (speed < 50)
    {
        arrow_increase(true);
    }
    else
    {
        arrow_increase(false);
    }
    if (speed > 80)
    {
        arrow_decrease(true);
    }
    else
    {
        arrow_decrease(false);
    }
}

void _handleIoInterrupt()
{
    // CRITICAL SECTION I2C: start
    xSemaphoreTake(i2c_mutex, portMAX_DELAY);

    PCF8574::DigitalInput dra = pcf8574.digitalReadAll();

    xSemaphoreGive(i2c_mutex);
    // CRITICAL SECTION I2C: end

    if ((dra.p1 & dra.p2 & dra.p3 & dra.p4 & dra.p5 & dra.p6 & dra.p7) == 1)
    {
        return;
    }
    Serial.printf("PCF: %d %d %d %d - %d %d %d %d\n", dra.p0, dra.p1, dra.p2, dra.p3, dra.p4, dra.p5, dra.p6, dra.p7);

    taskSleep = 10;

    if (!dra.p7 || !dra.p1)
    {
        taskSleep = 200;
        update_indicator(!dra.p7, !dra.p1);
    }
    if (!dra.p6)
    {
        taskSleep = 100;
        light1OnOff();
    }
    if (!dra.p5)
    {
        taskSleep = 100;
        light2OnOff();
    }

    // Simulation
    if (!dra.p4)
    {
        speed += 10;
        write_speed(speed);
        _speedCheck(speed);
    }
    if (!dra.p3)
    {
        speed -= 10;
        if (speed < 0)
        {
            speed = 0;
        }
        write_speed(speed);
        _speedCheck(speed);
    }

    // int v[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    // for (int idx = 0; idx < 8; idx++)
    // {
    //     v[idx] = pcf8574.digitalRead(idx);
    // }
    // for (int idx = 0; idx < 8; idx++)
    // {
    //     Serial.printf("%d:%d - ", idx, v[idx]);
    // }
    // Serial.println();
}

void set_ioext(int port, bool value)
{
    // check port
    if (port < 0 || port >= PCF8574_NUM_PORTS)
    {
        return;
    }

    // CRITICAL SECTION I2C: start
    xSemaphoreTake(i2c_mutex, portMAX_DELAY);

    pcf8574.digitalWrite(port, value);

    xSemaphoreGive(i2c_mutex);
    // CRITICAL SECTION I2C: end
}

int get_ioext(int port)
{
    // check port
    if (port < 0 || port >= PCF8574_NUM_PORTS)
    {
        return -1;
    }

    // CRITICAL SECTION I2C: start
    xSemaphoreTake(i2c_mutex, portMAX_DELAY);

    int value = pcf8574.digitalRead(P0);

    xSemaphoreGive(i2c_mutex);
    // CRITICAL SECTION I2C: end

    return value;
}

int counter = 0;
void io_ext_demo_task(void *pvParameter)
{
    // polling loop
    while (1)
    {
        if (counter > 10)
        {
            set_ioext(0, !get_ioext(0));
            counter = 0;
        }
        counter++;

        // handle inputs
        if (ioInterruptRequest)
        {
            _handleIoInterrupt();
            ioInterruptRequest = false;
        }
        // sleep
        vTaskDelay(taskSleep / portTICK_PERIOD_MS);
    }
}