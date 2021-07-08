//
// Software Serial (Interconnection to Telemetry / PC)
//

#include <definitions.h>

#include <SoftwareSerial.h>

#include "Serial.h"


void Uart::re_init() {
    init();
}

void Uart::init() {
    serial.begin(SERIAL_BAUDRATE, SWSERIAL_8N1, SERIAL_RX, SERIAL_TX);
    serial.enableIntTx(false);
}

extern Uart uart;
void serial_demo_task(void *pvParameter) {
  while (1) {
    // print heartbeat
    uart.serial.println("Hello from uart telemetry");

    // sleep for 1s
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
