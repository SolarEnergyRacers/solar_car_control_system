//
// Real-Time Clock
//

#include <definitions.h>

#include <I2CBus.h>

#include <Wire.h>

#include "RTC.h"

#include <RtcDS1307.h>

extern I2CBus i2cBus;

void RTC::re_init() { init(); }

void RTC::exit() {
  // TODO
}

void RTC::init(void) {

  // print compile time
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  printf("[RTC] Compile date/time: %02u/%02u/%04u %02u:%02u:%02u\n", compiled.Month(), compiled.Day(), compiled.Year(), compiled.Hour(),
         compiled.Minute(), compiled.Second());

  // CRITICAL SECTION I2C: start
  xSemaphoreTake(i2cBus.mutex, portMAX_DELAY);

  Rtc.Begin();

  // check validity and possibly update time
  if (!Rtc.IsDateTimeValid()) {
    // check & report error
    if (Rtc.LastError() != 0) {
      printf("[RTC] communication error %d\n", Rtc.LastError());
    } else {
      // Common Causes:
      //    1) first time you ran and the device wasn't running yet
      //    2) the battery on the device is low or even missing
      printf("[RTC] lost confidence. Set datetime to compile time of this "
             "binary.\n");
      Rtc.SetDateTime(compiled);
    }
  }

  // start device
  if (!Rtc.GetIsRunning()) {
    printf("[RTC] was not actively running, starting now\n");
    Rtc.SetIsRunning(true);
  }

  // check time
  RtcDateTime now = Rtc.GetDateTime();
  if (now < compiled) {
    printf("[RTC] rtc time older than compile time! Updating DateTime.\n");
    Rtc.SetDateTime(compiled);
  } else if (now > compiled) {
    printf("[RTC] rtc time newer than compile time.\n");
  } else if (now == compiled) {
    printf("[RTC] rtc time equal to compile time.\n");
  }

  // set pin
  Rtc.SetSquareWavePin(DS1307SquareWaveOut_Low);

  xSemaphoreGive(i2cBus.mutex);
  // CRITICAL SECTION I2C: end
}

RtcDateTime RTC::read_rtc_datetime(void) {

  // CRITICAL SECTION I2C: start
  xSemaphoreTake(i2cBus.mutex, portMAX_DELAY);

  // check connection & confidence
  if (!Rtc.IsDateTimeValid()) {
    if (Rtc.LastError() != 0) {
      // report
      printf("[RTC] i2c communications error: %d\n", Rtc.LastError());
    } else {
      // Common Causes:
      //   - the battery on the device is low or even missing and the power line
      //   was disconnected
      printf("[RTC] lost confidence in the datetime\n");
    }
  }

  // get datetime
  RtcDateTime now = Rtc.GetDateTime();

  xSemaphoreGive(i2cBus.mutex);
  // CRITICAL SECTION I2C: end

  return now;
}

void RTC::task() {

  while (1) {

    // get date & time
    RtcDateTime now = read_rtc_datetime();
    printf("[RTC] current datetime: %02u/%02u/%04u %02u:%02u:%02u\n", now.Month(), now.Day(), now.Year(), now.Hour(), now.Minute(),
           now.Second());

    // sleep for 1s
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}