//
// PCF8574 I/O Extension over I2C  !!! UNTESTED !!!
//

#include "../../include/definitions.h"
#include "PCF8574.h" // PCF8574
#include <I2CBus.h>
#include <Wire.h> // I2C

#include "DriverDisplay.h"
#include "IOExt.h"
#include "Indicator.h"

#define PCF8574_NUM_PORTS 8

PCF8574 pcf8574(I2C_ADDRESS_PCF8574, I2C_SDA, I2C_SCL, PCF8574_INTERRUPT_PIN,
                key_pressed_interrupt_handler);

int speed = 0;
int taskSleep = 10;

void init_ioext() {

  // CRITICAL SECTION I2C: start
  xSemaphoreTake(i2c_mutex, portMAX_DELAY);

  // setup pins
  pcf8574.pinMode(P0, INPUT);
  pcf8574.pinMode(P1, INPUT);
  pcf8574.pinMode(P2, INPUT);
  pcf8574.pinMode(P3, INPUT);
  pcf8574.pinMode(P4, INPUT);
  pcf8574.pinMode(P5, INPUT);
  pcf8574.pinMode(P6, INPUT);
  pcf8574.pinMode(P7, INPUT);

  // start
  if (pcf8574.begin()) {
    printf("[PCF8574] Init successful.\n");
  } else {
    printf("[PCF8574] Init failed.\n");
  }

  xSemaphoreGive(i2c_mutex);
  // CRITICAL SECTION I2C: end
}

// volatile PCF8574::DigitalInput digitalIn;
volatile bool ioInterruptRequest = false;
void key_pressed_interrupt_handler() {
  ioInterruptRequest = true;
  // digitalIn = pcf8574.digitalReadAll();
  // buf = readBuffer();
  // Wire.requestFrom(0x20,(uint8_t)1);
  // ioBuf = Wire.read();
}

void _speedCheck(int speed) {
  if (speed < 50) {
    arrow_increase(true);
  } else {
    arrow_increase(false);
  }
  if (speed > 80) {
    arrow_decrease(true);
  } else {
    arrow_decrease(false);
  }
}

void _handleIoInterrupt() {
  // CRITICAL SECTION I2C: start
  xSemaphoreTake(i2c_mutex, portMAX_DELAY);

  PCF8574::DigitalInput dra = pcf8574.digitalReadAll();

  xSemaphoreGive(i2c_mutex);
  // CRITICAL SECTION I2C: end

  if ((dra.p1 & dra.p2 & dra.p3 & dra.p4 & dra.p5 & dra.p6 & dra.p7) == 1) {
    return;
  }
  printf("PCF: %d %d %d %d - %d %d %d %d\n", dra.p0, dra.p1, dra.p2, dra.p3,
         dra.p4, dra.p5, dra.p6, dra.p7);

  taskSleep = 10;

  // turn indicator and hazard lights
  if (!dra.p7 || !dra.p1) {
    taskSleep = 200;
    update_indicator(!dra.p7, !dra.p1);
  }
  // position lights on/off
  if (!dra.p6) {
    taskSleep = 100;
    light1OnOff();
  }
  // head lights on/off
  if (!dra.p5) {
    taskSleep = 100;
    light2OnOff();
  }

  // Simulation
  if (!dra.p4) {
    speed += 10;
    write_speed(speed);
    _speedCheck(speed);
  }
  if (!dra.p3) {
    speed -= 10;
    if (speed < 0) {
      speed = 0;
    }
    write_speed(speed);
    _speedCheck(speed);
  }
#ifdef DEBUGLEVEL_VERBOSE == true
  int v[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  for (int idx = 0; idx < 8; idx++) {
    v[idx] = pcf8574.digitalRead(idx);
  }
  for (int idx = 0; idx < 8; idx++) {
    printf("%d:%d - ", idx, v[idx]);
  }
  Serial.println();
#endif
}

void set_ioext(int port, bool value) {
  // check port
  if (port < 0 || port >= PCF8574_NUM_PORTS) {
    return;
  }

  // CRITICAL SECTION I2C: start
  xSemaphoreTake(i2c_mutex, portMAX_DELAY);

  pcf8574.digitalWrite(port, value);

  xSemaphoreGive(i2c_mutex);
  // CRITICAL SECTION I2C: end
}

int get_ioext(int port) {
  // check port
  if (port < 0 || port >= PCF8574_NUM_PORTS) {
    return -1;
  }

  // CRITICAL SECTION I2C: start
  xSemaphoreTake(i2c_mutex, portMAX_DELAY);

  int value = pcf8574.digitalRead(P0);

  xSemaphoreGive(i2c_mutex);
  // CRITICAL SECTION I2C: end

  return value;
}

void io_ext_demo_task(void *pvParameter) {

  // polling loop
  while (1) {
    // handle input interrupts
    if (ioInterruptRequest) {
      _handleIoInterrupt();
      ioInterruptRequest = false;
    }
    // sleep
    vTaskDelay(taskSleep / portTICK_PERIOD_MS);
  }
}