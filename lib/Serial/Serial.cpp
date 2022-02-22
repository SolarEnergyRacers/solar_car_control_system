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
  Serial << "[?] Setup " << getName().c_str() << "..." << endl;
  Serial << "S0 ----HC-12------------" << endl;
  Serial << "S0 Serial TX0 is on pin: " << TX << endl;
  Serial << "S0 Serial RX0 is on pin: " << RX << endl;
  Serial << "S0 Serial Baud Rate:     " << SERIAL_BAUDRATE << endl;
  Serial << "S0 --------------------" << endl;

  // Serial2.begin(SERIAL2_BAUDRATE, SERIAL_8N1, SERIAL2_RX, SERIAL2_TX);
  // Serial2.enableIntTx(false);
  delay(1000);
  Serial << "S0 Serial2 TX2 is on pin: " << SERIAL2_TX << endl;
  Serial << "S0 Serial2 RX2 is on pin: " << SERIAL2_RX << endl;
  Serial << "S0 Serial2 Baud Rate:     " << SERIAL2_BAUDRATE << endl;
  Serial2 << endl;
  Serial2 << "S2--HC-12------------------" << endl;
  Serial2 << "S2 Serial2 TX2 is on pin: " << SERIAL2_TX << endl;
  Serial2 << "S2 Serial2 RX2 is on pin: " << SERIAL2_RX << endl;
  Serial2 << "S2 Serial2 Baud Rate:     " << SERIAL2_BAUDRATE << endl;
}

// bool payload1;
// bool payload2;
void Uart::task() {
  while (1) {
    // payload1 = payload2 = false;
    while (Serial2.available()) {
      // Serial.print(char(Serial2.read()));
      Serial << char(Serial2.read());
      // payload2 = true;
    }
    // if (payload2) {
    //   Serial << "---read" << endl;
    // }
    while (Serial.available()) {
      // Serial2.print(char(Serial.read()));
      Serial2 << Serial.readString();
    }
    delay(100);
  }
}
