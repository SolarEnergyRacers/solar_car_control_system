//
// I2C Bus
//
#include <definitions.h>

#include <fmt/core.h>
#include <iostream>

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h> // semaphore

#include <Wire.h> // Arduino I2C library

#include <I2CBus.h>

using namespace std;

void I2CBus::re_init() { init(); }

void I2CBus::init(void) {
  cout << "[?] Init '" << getName() << "'" << endl;

  mutex = xSemaphoreCreateMutex();
  // init i2c wire library
  Wire.begin(I2C_SDA, I2C_SCL, I2C_FREQ);
  xSemaphoreGive(mutex);

  cout << "[v] I2C inited: I2C_SDA=" << I2C_SDA << ", I2C_SCL=" << I2C_SCL << ", I2C_FREQ=" << I2C_FREQ << "." << endl;
  scan_i2c_devices();
}

// test if the i2c bus is available and ready for transaction at address adr
bool I2CBus::i2c_available(uint8_t adr) {
  uint32_t timeout = millis();
  bool ready = false;

  xSemaphoreTake(mutex, portMAX_DELAY);
  while ((millis() - timeout < 100) && (!ready)) {
    Wire.beginTransmission(adr);
    ready = (Wire.endTransmission() == 0);
  }
  xSemaphoreGive(mutex);

  return ready;
}

void I2CBus::scan_i2c_devices() {
  /* I2C slave Address Scanner
  for 5V bus
      * Connect a 4.7k resistor between SDA and Vcc
      * Connect a 4.7k resistor between SCL and Vcc
  for 3.3V bus
      * Connect a 2.4k resistor between SDA and Vcc
      * Connect a 2.4k resistor between SCL and Vcc
  */
  cout << "    Scanning I2C addresses:" << endl << "    ";
  uint8_t cnt = 0;
  string s;

  xSemaphoreTake(mutex, portMAX_DELAY);
  for (uint8_t addr = 0; addr < 0x80; addr++) {
    uint8_t ec = -1;
    try {
      Wire.beginTransmission(addr);
      ec = Wire.endTransmission(true);
    } catch (__exception ex) {
      // do nothing
    }
    if (ec == 0) {
      s = fmt::format("{:02x} ", addr);
      cnt++;
    } else {
      s = "-- ";
    }
    if ((addr & 0x0f) == 0x0f) {
      s.append("\n    ");
    }
    cout << s;
  }
  xSemaphoreGive(mutex);

  cout << "Scan completed: " << cnt << " I2C devices found." << endl;
}
