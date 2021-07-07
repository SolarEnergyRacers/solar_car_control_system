//
// Software Serial (Interconnection to Telemetry / PC)
//

#include <definitions.h>

#include <SoftwareSerial.h>

#include "Serial.h"

SoftwareSerial uart;

void init_serial() {
  uart.begin(SERIAL_BAUDRATE, SWSERIAL_8N1, SERIAL_RX, SERIAL_TX);
  uart.enableIntTx(false);
}

void serial_demo_task(void *pvParameter) {
  while (1) {
    // print heartbeat
    uart.println("Hello from uart telemetry");

    // sleep for 1s
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
