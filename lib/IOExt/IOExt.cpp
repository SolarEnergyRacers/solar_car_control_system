//
// PCF8574 I/O Extension over I2C
//

#include <PCF8574.h> // PCF8574
#include <Wire.h>    // I2C
#include <definitions.h>

#include <DriverDisplayC.h>
#include <I2CBus.h>
#include <Indicator.h>

#include <IOExt.h>

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
  IOExt2.pinMode(P2, INPUT); // driving lights
  IOExt2.pinMode(P3, INPUT); // position lights
  IOExt2.pinMode(P4, INPUT); // horn
  IOExt2.pinMode(P5, INPUT); // next screen
  IOExt2.pinMode(P6, INPUT); // set speed or power mode
  IOExt2.pinMode(P7, INPUT); // set tempomat/poweromat

  // setup pins
  IOExt3.pinMode(P0, INPUT);
  IOExt3.pinMode(P1, INPUT); 
  IOExt3.pinMode(P2, INPUT); 
  IOExt3.pinMode(P3, INPUT); 
  IOExt3.pinMode(P4, INPUT); 
  IOExt3.pinMode(P5, INPUT); 
  IOExt3.pinMode(P6, INPUT); 
  IOExt3.pinMode(P7, INPUT); 

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

void IOExt::handleIoInterrupt() {
  // CRITICAL SECTION I2C: start
  xSemaphoreTake(i2cBus.mutex, portMAX_DELAY);
  PCF8574::DigitalInput dra2 = IOExt2.digitalReadAll();
  PCF8574::DigitalInput dra3 = IOExt3.digitalReadAll();
  xSemaphoreGive(i2cBus.mutex);

  if (dra2.p0 & dra2.p1 & dra2.p2 & dra2.p3 & dra2.p4 & dra2.p5 & dra2.p6 & dra2.p7
     &dra3.p0 & dra3.p1 & dra3.p2 & dra3.p3 & dra3.p4 & dra3.p5 & dra3.p6 & dra3.p7) {
    taskSleep = 50; // default value for fast reaction to pressed button
    return;
  }

  taskSleep = 100; // debounce button

  //debug_printf("(%ldms)\t", millis());
  debug_printf("PCF2: %d %d %d %d - %d %d %d %d\n", dra2.p0, dra2.p1, dra2.p2, dra2.p3, dra2.p4, dra2.p5, dra2.p6, dra2.p7);
  debug_printf("PCF3: %d %d %d %d - %d %d %d %d\n", dra3.p0, dra3.p1, dra3.p2, dra3.p3, dra3.p4, dra3.p5, dra3.p6, dra3.p7);

  // PCF8574:2 pin assignment
  bool left = !dra2.p0;
  bool right = !dra2.p1;
  bool drivingLights = !dra2.p2;
  bool positionLights = !dra2.p3;
  bool horn = !dra2.p4;
  bool nextScreen = !dra2.p5;
  bool speedPowerControlMode = !dra2.p6;
  bool speedPowerControlOnOff = !dra2.p7;

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