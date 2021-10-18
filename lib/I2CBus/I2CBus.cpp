//
// I2C Bus
//

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h> // semaphore

#include <Wire.h> // Arduino I2C library

#include <definitions.h>

#include <I2CBus.h>

void I2CBus::re_init() { init(); }

void I2CBus::init(void) {

  // CRITICAL SECTION I2C: start
  // init mutex (it is acquired)
  mutex = xSemaphoreCreateBinary();

  // init i2c wire library
  Wire.begin(I2C_SDA, I2C_SCL, I2C_FREQ);

  xSemaphoreGive(mutex);
  // CRITICAL SECTION I2C: end

  printf("[v] I2C inited: I2C_SDA=%d, I2C_SCL=%d, I2C_FREQ=%d.\n", I2C_SDA, I2C_SCL, I2C_FREQ);
  scan_i2c_devices();
}

// test if the i2c bus is available and ready for transaction at address adr
bool I2CBus::i2c_available(uint8_t adr) {
  uint32_t timeout = millis();
  bool ready = false;

  // CRITICAL SECTION I2C: start
  xSemaphoreTake(mutex, portMAX_DELAY);

  while ((millis() - timeout < 100) && (!ready)) {
    Wire.beginTransmission(adr);
    ready = (Wire.endTransmission() == 0);
  }

  xSemaphoreGive(mutex);
  // CRITICAL SECTION I2C: end

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
  printf("    Scanning I2C addresses:\n    ");

  // CRITICAL SECTION I2C: start
  xSemaphoreTake(mutex, portMAX_DELAY);

  uint8_t cnt = 0;
  for (uint8_t i = 0; i < 0x80; i++) {
    Wire.beginTransmission(i);
    uint8_t ec = Wire.endTransmission(true);
    if (ec == 0) {
      printf("%02x ", i);
      cnt++;
    } else {
      printf("-- ");
    }
    if ((i & 0x0f) == 0x0f) {
      printf("\n    ");
    }
  }

  xSemaphoreGive(mutex);
  // CRITICAL SECTION I2C: end

  printf("Scan completed: %d I2C devices found.\n", cnt);
}
