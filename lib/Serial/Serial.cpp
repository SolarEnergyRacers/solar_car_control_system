//
// Software Serial (Interconnection to Telemetry / PC)
//

#include <definitions.h>

#include <SoftwareSerial.h>
#include <Streaming.h>
#include <stdlib.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <Console.h>
#include <abstract_task.h>

using namespace std;

#include <Serial.h>

extern Console console;

void Uart::re_init() { init(); }
void Uart::exit() {}

void Uart::init() {
  // Serial.begin(SERIAL_BAUDRATE);
  console << "[?] Setup " << getName().c_str() << "..."
          << "\n";
  console << "S0 ----HC-12------------"
          << "\n";
  console << "S0 Serial TX0 is on pin: " << TX << "\n";
  console << "S0 Serial RX0 is on pin: " << RX << "\n";
  console << "S0 Serial Baud Rate:     " << SERIAL_BAUDRATE << "\n";
  console << "S0 --------------------"
          << "\n";

  // Serial2.begin(SERIAL2_BAUDRATE, SERIAL_8N1, SERIAL2_RX, SERIAL2_TX);
  // Serial2.enableIntTx(false);
  delay(1000);
  console << "S2 Serial2 TX2 is on pin: " << SERIAL2_TX << "\n";
  console << "S2 Serial2 RX2 is on pin: " << SERIAL2_RX << "\n";
  console << "S2 Serial2 Baud Rate:     " << SERIAL2_BAUDRATE << "\n";
  console << "\n";
}

// bool payload1;
// bool payload2;
void Uart::task() {
  while (1) {
    // // payload1 = payload2 = false;
    // while (Serial2.available()) {
    //   // Serial.print(char(Serial2.read()));
    //   Serial << char(Serial2.read());
    //   // payload2 = true;
    // }
    // // if (payload2) {
    // //   Serial << "---read" << endl;
    // // }
    // while (Serial.available()) {
    //   // Serial2.print(char(Serial.read()));
    //   Serial2 << Serial.readString();
    // }
    delay(100);
  }
}
