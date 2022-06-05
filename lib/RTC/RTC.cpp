//
// Real-Time Clock
//

#include <definitions.h>

#include <fmt/core.h>
#include <fmt/printf.h>
#include <iostream>
#include <stdio.h>
#include <string>

#include <Console.h>
#include <DriverDisplay.h>
#include <ESP32Time.h>
#include <Helper.h>
#include <I2CBus.h>
#include <RTC.h>
#include <RtcDS1307.h>
#include <Wire.h>

extern Console console;
extern ESP32Time esp32time;
extern I2CBus i2cBus;
extern DriverDisplay driverDisplay;

// ------------------
// FreeRTOS functions

string RTC::re_init() { return init(); }

string RTC::init(void) {
  bool hasError = false;
  console << "[??] Init 'RTC'...\n";
  console << fmt::format("    DS1307_ADDRESS {:02x}\n", DS1307_ADDRESS);
  // print compile time
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  console << fmt::format("    [INFO] rtc compile date/time: {:02}/{:02}/{:04} {:02}:{:02}:{:02}\n", compiled.Month(), compiled.Day(),
                         compiled.Year(), compiled.Hour(), compiled.Minute(), compiled.Second());
  xSemaphoreTakeT(i2cBus.mutex);
  Rtc.Begin();
  // check validity and possibly update time
  bool isValid = Rtc.IsDateTimeValid();
  xSemaphoreGive(i2cBus.mutex);
  if (!isValid) {
    // check & report error
    xSemaphoreTakeT(i2cBus.mutex);
    uint8_t lastError = Rtc.LastError();
    xSemaphoreGive(i2cBus.mutex);
    if (lastError != 0) {
      console << "    [INFO] rtc Communication error " << Rtc.LastError() << "\n";
    } else {
      // Common Causes:
      //    1) first time you ran and the device wasn't running yet
      //    2) the battery on the device is low or even missing
      console << "    [WARN] rtc lost confidence. Set datetime to compile time of this binary.\n";
      xSemaphoreTakeT(i2cBus.mutex);
      Rtc.SetDateTime(compiled);
      xSemaphoreGive(i2cBus.mutex);
    }
  }
  // ------------------

  // start device
  xSemaphoreTakeT(i2cBus.mutex);
  bool isRunning = Rtc.GetIsRunning();
  xSemaphoreGive(i2cBus.mutex);
  if (!isRunning) {
    console << "    [INFO] rtc was not actively running, starting now\n";
    xSemaphoreTakeT(i2cBus.mutex);
    Rtc.SetIsRunning(true);
    xSemaphoreGive(i2cBus.mutex);
  }

  // check time
  xSemaphoreTakeT(i2cBus.mutex);
  RtcDateTime now = Rtc.GetDateTime();
  xSemaphoreGive(i2cBus.mutex);
  if (now < compiled) {
    console << "    [INFO] rtc time older than compile time! Updating DateTime.\n";
    xSemaphoreTakeT(i2cBus.mutex);
    Rtc.SetDateTime(compiled);
    xSemaphoreGive(i2cBus.mutex);
  } else if (now > compiled) {
    console << "    [INFO] rtc time newer than compile time.\n";
  } else if (now == compiled) {
    console << "    [INFO] rtc time equal to compile time.\n";
  }

  // set pin
  xSemaphoreTakeT(i2cBus.mutex);
  Rtc.SetSquareWavePin(DS1307SquareWaveOut_Low);
  xSemaphoreGive(i2cBus.mutex);

  return fmt::format("[{}] RTC initialized.", hasError ? "--" : "ok");
}


void RTC::exit() {
  // TODO
}

RtcDateTime RTC::read_rtc_datetime(void) {
  RtcDateTime now;
  console << "----01----";
  // check connection & confidence
  xSemaphoreTakeT(i2cBus.mutex);
  bool isValid = Rtc.IsDateTimeValid();
  xSemaphoreGive(i2cBus.mutex);
  if (!isValid) {
    uint8_t lastError = Rtc.LastError();
    xSemaphoreGive(i2cBus.mutex);
    console << "----02----";
    if (lastError != 0) {
      // report
      console << "[RTC] i2c communications error: " << Rtc.LastError() << "\n";
    } else {
      // Common Causes:
      //   - the battery on the device is low or even missing and the power line
      //   was disconnected
      console << "[RTC] lost confidence in the datetime\n";
    }
  }
  console << "----03----";
  // get datetime
  xSemaphoreTakeT(i2cBus.mutex);
  now = Rtc.GetDateTime();
  xSemaphoreGive(i2cBus.mutex);
  console << "----04----";
  return now;
}

void RTC::task() {

  while (1) {

    // RtcDateTime now = read_rtc_datetime();

    // // setTime(30, 24, 15, 17, 1, 2021); // 17th Jan 2021 15:24:30
    // esp32time.setTime(now.Second(), now.Minute(), now.Hour(), now.Day(), now.Month(), now.Year()); // 17th Jan 2021 15:24:30

    vTaskDelay(sleep_polling_ms / portTICK_PERIOD_MS);
  }
}