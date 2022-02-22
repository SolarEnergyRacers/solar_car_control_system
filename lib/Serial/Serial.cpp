//
// Software Serial (Interconnection to Telemetry / PC)
//

#include <definitions.h>

#include <SoftwareSerial.h>
#include <Streaming.h>
#include <stdlib.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <abstract_task.h>

using namespace std;

#include <Serial.h>

void Uart::re_init() { init(); }
void Uart::exit() {}

void Uart::init() {
  Serial2.begin(SERIAL_BAUDRATE, SWSERIAL_8N1, SERIAL_RX, SERIAL_TX);
  //Serial2.enableIntTx(false);
  delay(500);
  Serial2 << endl;
  Serial2 << "S2--------------------" << endl;
  Serial2 << "S2esp32dev + free RTOS" << endl;
  Serial2 << "S2Solar Energy Car Races SER4" << VERSION_PUBLISHED << endl;
  Serial2 << "S2--------------------" << endl;
  Serial << endl;
  Serial << "S--------------------" << endl;
  Serial << "Sesp32dev + free RTOS" << endl;
  Serial << "SSolar Energy Car Races SER4" << VERSION_PUBLISHED << endl;
  Serial << "S--------------------" << endl;
}

void Uart::task() {
  bool payload;
  while (1) {
    Serial << ".";
    payload = false;
    while (Serial2.available()) {
      Serial.print(char(Serial2.read()));
      payload = true;
    }
    if (payload) {
      Serial.println();
    }
    String buffer = Serial.readString();
    if (buffer.length() > 0)
      Serial2 << buffer << " -- sent!" << endl;
    delay(100);
  }
}
