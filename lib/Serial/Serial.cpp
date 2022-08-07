//
// Software Serial (Interconnection to Telemetry / PC)
//

#include <definitions.h>

#include <fmt/core.h>
#include <inttypes.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <Console.h>
#include <DriverDisplay.h>
#include <abstract_task.h>

using namespace std;

#include <Console.h>
#include <Serial.h>
#include <SoftwareSerial.h>
#include <Streaming.h>

extern Console console;
extern DriverDisplay driverDisplay;
extern Console console;

string Uart::re_init() { return init(); }

string Uart::init() {
  bool hasError = false;
  console = Console();
  // abstract_task::init();
  //  init serial for console IO
  Serial.begin(carState.Serial1Baudrate);
  delay(1000);
  console << "[  ] Init 'UartSerial'...\n";
  console << "     ---Serial------------\n";
  console << "     Serial TX0 is on pin: " << to_string(TX) << "\n";
  console << "     Serial RX0 is on pin: " << to_string(RX) << "\n";
  console << "     Serial Baud Rate:     " << carState.Serial1Baudrate << "\n";
  // init serial for radio console IO
  Serial2.begin(carState.Serial2Baudrate, SERIAL_8N1, SERIAL2_RX, SERIAL2_TX);
  // Serial2.enableIntTx(false);
  delay(1000);
  // both Serial and Serial2 inited -> from now the Console class is usable
  console << "     ---Serial2 HC-12------\n";
  console << "     Serial2 TX2 is on pin: " << to_string(SERIAL2_TX) << "\n";
  console << "     Serial2 RX2 is on pin: " << to_string(SERIAL2_RX) << "\n";
  console << "     Serial2 Baud Rate:     " << carState.Serial2Baudrate << "\n";
  console << "     done.\n";
  return fmt::format("[{}] Serial and Serial2 initialized.", hasError ? "--" : "ok");
}

void Uart::exit() {}

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
    // //   Serial << "---read\n";
    // // }
    // while (Serial.available()) {
    //   // Serial2.print(char(Serial.read()));
    //   Serial2 << Serial.readString();
    // }
    vTaskDelay(sleep_polling_ms / portTICK_PERIOD_MS);
  }
}
