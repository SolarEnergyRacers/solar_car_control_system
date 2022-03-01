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

#include <Serial.h>
#include <SoftwareSerial.h>
#include <Streaming.h>

extern DriverDisplay driverDisplay;
extern Console console;

void Uart::re_init() { init(); }
void Uart::exit() {}

void Uart::init() {
  console = Console();
  // init serial for console IO
  Serial.begin(SERIAL_BAUDRATE);
  delay(1000);
  cout << "[?] Setup '" << getName() << "'...\n";
  cout << "    ---Serial------------\n";
  cout << "    Serial TX0 is on pin: " << to_string(TX) << "\n";
  cout << "    Serial RX0 is on pin: " << to_string(RX) << "\n";
  cout << "    Serial Baud Rate:     " << SERIAL_BAUDRATE << "\n";
  // init serial for radio console IO
  Serial2.begin(SERIAL2_BAUDRATE, SERIAL_8N1, SERIAL2_RX, SERIAL2_TX);
  // Serial2.enableIntTx(false);
  delay(1000);
  // both Serial and Serial2 inited -> from now the Console class is usable
  cout << "    ---Serial2 HC-12------\n";
  cout << "    Serial2 TX2 is on pin: " << to_string(SERIAL2_TX) << "\n";
  cout << "    Serial2 RX2 is on pin: " << to_string(SERIAL2_RX) << "\n";
  cout << "    Serial2 Baud Rate:     " << SERIAL2_BAUDRATE << "\n";

  cout << fmt::format("[v] {} inited.\n", getName());
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
