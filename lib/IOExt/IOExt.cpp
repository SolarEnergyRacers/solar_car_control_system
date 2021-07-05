//
// PCF8574 I/O Extension over I2C
//

#include "../../include/definitions.h"
#include "PCF8574.h" // PCF8574
#include <I2CBus.h>
#include <Wire.h> // I2C

#include "DriverDisplayC.h"
#include "IOExt.h"
#include "Indicator.h"

#define PCF8574_NUM_PORTS 8

PCF8574 IOExt2(I2C_ADDRESS_PCF8574_IOExt2, I2C_SDA, I2C_SCL, I2C_INTERRUPT_PIN_PCF8574, keyPressedInterruptHandler);

// simulation - start (for simulation purpose)
int speed = 0;
int acceleration = 0;
// simulation - end
int taskSleep = 50;

void init_IOExt2() {

  // CRITICAL SECTION I2C: start
  xSemaphoreTake(i2c_mutex, portMAX_DELAY);

  // setup pins
  IOExt2.pinMode(P0, INPUT); // left indicator
  IOExt2.pinMode(P1, INPUT); // right indicator
  IOExt2.pinMode(P2, INPUT); // set tempomat/poweromat
  IOExt2.pinMode(P3, INPUT); // set tempo or power mode
  IOExt2.pinMode(P4, INPUT); // horn
  IOExt2.pinMode(P5, INPUT); // position lights
  IOExt2.pinMode(P6, INPUT); // driving lights
  IOExt2.pinMode(P7, INPUT); // next screen

  // start
  if (IOExt2.begin()) {
    printf("[v] IOExt2 inited.\n");
  } else {
    printf("[x] IOExt2 init failed.\n");
    // TODO: action for init error?
  }

  xSemaphoreGive(i2c_mutex);
  // CRITICAL SECTION I2C: end
}

volatile bool ioInterruptRequest = false;
void keyPressedInterruptHandler() { ioInterruptRequest = true; }

void _speedCheck(int speed) {
  DriverDisplayC *dd = DriverDisplayC::instance();
  if (speed < 50) {
    dd->arrow_increase(true);
  } else {
    dd->arrow_increase(false);
  }
  if (speed > 80) {
    dd->arrow_decrease(true);
  } else {
    dd->arrow_decrease(false);
  }
}

void _handleIoInterrupt() {
  // CRITICAL SECTION I2C: start
  xSemaphoreTake(i2c_mutex, portMAX_DELAY);

  PCF8574::DigitalInput dra = IOExt2.digitalReadAll();

  if (dra.p0 & dra.p1 & dra.p2 & dra.p3 & dra.p4 & dra.p5 & dra.p6 & dra.p7) {
    taskSleep = 50;            // default value for fast reaction to pressed button
    xSemaphoreGive(i2c_mutex); /// TODO_KSC:move directly after digitalReadAll
    // CRITICAL SECTION I2C: end
    return;
  }

  taskSleep = 100; // debounce button

  printf("PCF %ld: %d %d %d %d - %d %d %d %d\n", millis(), dra.p0, dra.p1, dra.p2, dra.p3, dra.p4, dra.p5, dra.p6, dra.p7);

  bool left = !dra.p0;
  bool right = !dra.p1;
  bool speedPowerControlOnOff = !dra.p2;
  bool speedPowerControlMode = !dra.p3;
  bool horn = !dra.p4;
  bool positionLights = !dra.p5;
  bool drivingLights = !dra.p6;
  bool nextScreen = !dra.p7;

  DriverDisplayC *dd = DriverDisplayC::instance();

  // turn indicator and hazard lights
  if (left && right) {
    setIndicator(INDICATOR::WARN);
  } else if (left && !right) {
    setIndicator(INDICATOR::LEFT);
  } else if (!left && right) {
    setIndicator(INDICATOR::RIGHT);
  }
  if (positionLights) {
    dd->light1OnOff();
  }
  if (drivingLights) {
    dd->light2OnOff();
  }

  // Simulation
  if (speedPowerControlOnOff) {
    speed += 10;
    dd->write_speed(speed);
    _speedCheck(speed);
  }
  if (speedPowerControlMode) {
    speed -= 10;
    if (speed < 0) {
      speed = 0;
    }
    dd->write_speed(speed);
    _speedCheck(speed);
  }
  if (horn) {
    acceleration += 10;
    dd->write_acceleration(acceleration);
  }
  if (nextScreen) {
    acceleration -= 10;
    if (acceleration < 0) {
      acceleration = 0;
    }
    dd->write_acceleration(acceleration);
  }
  xSemaphoreGive(i2c_mutex); // TODO_KSC: remove
  // CRITICAL SECTION I2C: end
}

void set_ioext(int port, bool value) {
  // check port
  if (port < 0 || port >= PCF8574_NUM_PORTS) {
    return;
  }

  // CRITICAL SECTION I2C: start
  xSemaphoreTake(i2c_mutex, portMAX_DELAY);

  IOExt2.digitalWrite(port, value);

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

  int value = IOExt2.digitalRead(P0);

  xSemaphoreGive(i2c_mutex);
  // CRITICAL SECTION I2C: end

  return value;
}

bool isInInterruptHandler = false;
void IOExt2_task(void *pvParameter) {

  // polling loop
  while (1) {
    // handle input interrupts
    if (ioInterruptRequest && !isInInterruptHandler) {
      isInInterruptHandler = true;
      _handleIoInterrupt();
      ioInterruptRequest = false;
      isInInterruptHandler = false;
    }
    // sleep
    vTaskDelay(taskSleep / portTICK_PERIOD_MS);
  }
}