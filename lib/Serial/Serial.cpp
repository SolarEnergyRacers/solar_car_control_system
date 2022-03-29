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

void Uart::re_init() { init(); }
void Uart::exit() {}

void Uart::init() {
  console = Console();
  // init serial for console IO
  Serial.begin(SERIAL_BAUDRATE);
  delay(1000);
  console << "[?] Setup '" << getName() << "'...\n";
  console << "    ---Serial------------\n";
  console << "    Serial TX0 is on pin: " << to_string(TX) << "\n";
  console << "    Serial RX0 is on pin: " << to_string(RX) << "\n";
  console << "    Serial Baud Rate:     " << SERIAL_BAUDRATE << "\n";
  // init serial for radio console IO
  Serial2.begin(SERIAL2_BAUDRATE, SERIAL_8N1, SERIAL2_RX, SERIAL2_TX);
  // Serial2.enableIntTx(false);
  delay(1000);
  // both Serial and Serial2 inited -> from now the Console class is usable
  console << "    ---Serial2 HC-12------\n";
  console << "    Serial2 TX2 is on pin: " << to_string(SERIAL2_TX) << "\n";
  console << "    Serial2 RX2 is on pin: " << to_string(SERIAL2_RX) << "\n";
  console << "    Serial2 Baud Rate:     " << SERIAL2_BAUDRATE << "\n";

  console << fmt::format("[v] {} inited.\n", getName());
}

void Uart::task() {
  while (1) {
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
