//
// PCF8574 I/O Extension over I2C
//

#include <PCF8574.h> // PCF8574
#include <Wire.h>    // I2C
#include <definitions.h>

#include <DriverDisplayC.h>
#include <I2CBus.h>
#include <Indicator.h>

#include "IOExt.h"

extern I2CBus i2cBus;
extern Indicator indicator;

extern CONSTANT_MODE constant_drive_mode;
extern bool constant_drive_set;

void IOExt::re_init() { init(); }

void IOExt::init() {

  // CRITICAL SECTION I2C: start
  xSemaphoreTake(i2cBus.mutex, portMAX_DELAY);

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

  xSemaphoreGive(i2cBus.mutex);
  // CRITICAL SECTION I2C: end
}

void IOExt::exit(void) {
  // TODO
}

void IOExt::speedCheck(int speed) {
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

void IOExt::handleIoInterrupt() {
  // CRITICAL SECTION I2C: start
  xSemaphoreTake(i2cBus.mutex, portMAX_DELAY);
  PCF8574::DigitalInput dra = IOExt2.digitalReadAll();
  xSemaphoreGive(i2cBus.mutex);

  if (dra.p0 & dra.p1 & dra.p2 & dra.p3 & dra.p4 & dra.p5 & dra.p6 & dra.p7) {
    taskSleep = 50; // default value for fast reaction to pressed button
    return;
  }

  taskSleep = 100; // debounce button

  debug_printf("PCF (%ldms): %d %d %d %d - %d %d %d %d\n", millis(), dra.p0, dra.p1, dra.p2, dra.p3, dra.p4, dra.p5, dra.p6, dra.p7);

  // PCF8574:2 pin assignment
  bool left = !dra.p0;
  bool right = !dra.p3;
  bool speedPowerControlOnOff = !dra.p5;
  bool speedPowerControlMode = !dra.p4;
  bool horn = !dra.p6;
  bool positionLights = !dra.p2;
  bool drivingLights = !dra.p1;
  bool nextScreen = !dra.p7;

  DriverDisplayC *dd = DriverDisplayC::instance();

  // turn indicator and hazard lights
  if (left && right) {
    indicator.setIndicator(INDICATOR::WARN);
  } else if (left && !right) {
    indicator.setIndicator(INDICATOR::LEFT);
  } else if (!left && right) {
    indicator.setIndicator(INDICATOR::RIGHT);
  }
  if (positionLights) {
    printf("Position lights on/off\n");
    dd->light1OnOff();
  }
  if (drivingLights) {
    printf("Driving lights on/off\n");
    dd->light2OnOff();
  }
  if (speedPowerControlOnOff) {
    if (constant_drive_set) {
      dd->constant_drive_off();
      printf("Constand mode OFF\n");
    } else {
      dd->constant_drive_on();
      printf("Constand mode ON\n");
    }
  }
  if (speedPowerControlMode) {
    if (constant_drive_mode == CONSTANT_MODE::SPEED) {
      dd->constant_drive_mode_set(CONSTANT_MODE::POWER);
      printf("Constant mode switch to POWER\n");
    } else {
      dd->constant_drive_mode_set(CONSTANT_MODE::SPEED);
      printf("Constant mode switch to SPEED\n");
    }
  }
  if (horn) {
    printf("Horn\n");
  }
  if (nextScreen) {
    printf("Next screen\n");
  }

  // // Simulation
  // if (speedPowerControlOnOff) {
  //   speed += 10;
  //   dd->write_speed(speed);
  //   speedCheck(speed);
  // }
  // if (speedPowerControlMode) {
  //   speed -= 10;
  //   if (speed < 0) {
  //     speed = 0;
  //   }
  //   dd->write_speed(speed);
  //   speedCheck(speed);
  // }
  // if (horn) {
  //   acceleration += 10;
  //   dd->write_acceleration(acceleration);
  // }
  // if (nextScreen) {
  //   acceleration -= 10;
  //   if (acceleration < 0) {
  //     acceleration = 0;
  //   }
  //   dd->write_acceleration(acceleration);
  // }
}

void IOExt::set_ioext(int port, bool value) {
  // check port
  if (port < 0 || port >= PCF8574_NUM_PORTS) {
    return;
  }

  xSemaphoreTake(i2cBus.mutex, portMAX_DELAY);
  IOExt2.digitalWrite(port, value);
  xSemaphoreGive(i2cBus.mutex);
}

int IOExt::get_ioext(int port) {
  // check port
  if (port < 0 || port >= PCF8574_NUM_PORTS) {
    return -1;
  }

  xSemaphoreTake(i2cBus.mutex, portMAX_DELAY);
  int value = IOExt2.digitalRead(port);
  xSemaphoreGive(i2cBus.mutex);

  return value;
}

volatile bool IOExt::ioInterruptRequest = false;

void IOExt::task() {

  // polling loop
  while (1) {
    // handle input interrupts
    if (ioInterruptRequest && !isInInterruptHandler) {
      isInInterruptHandler = true;
      handleIoInterrupt();
      ioInterruptRequest = false;
      isInInterruptHandler = false;
    }
    // sleep
    vTaskDelay(taskSleep / portTICK_PERIOD_MS);
  }
}