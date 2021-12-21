//
// Real-Time Clock
//

#include <definitions.h>

#include <DriverDisplay.h>
#include <ESP32Time.h>
#include <Helper.h>
#include <I2CBus.h>
#include <RTC.h>
#include <RtcDS1307.h>
#include <Wire.h>

extern ESP32Time esp32time;
extern I2CBus i2cBus;
extern DriverDisplay driverDisplay;

// ------------------
// FreeRTOS functions

void RTC::re_init() { init(); }

void RTC::exit() {
  // TODO
}

void RTC::init(void) {
  char msg[100];
  printf("[?] Init 'RTC'...\n");
  // print compile time
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  printf("    [INFO] rtc compile date/time: %02u/%02u/%04u %02u:%02u:%02u\n", compiled.Month(), compiled.Day(), compiled.Year(),
         compiled.Hour(), compiled.Minute(), compiled.Second());

  xSemaphoreTakeT(i2cBus.mutex);
  Rtc.Begin();

  // check validity and possibly update time
  if (!Rtc.IsDateTimeValid()) {
    // check & report error
    if (Rtc.LastError() != 0) {
      printf("    [INFO] rtc Communication error %d\n", Rtc.LastError());
    } else {
      // Common Causes:
      //    1) first time you ran and the device wasn't running yet
      //    2) the battery on the device is low or even missing
      printf("    [WARN] rtc lost confidence. Set datetime to compile time of this binary.\n");
      Rtc.SetDateTime(compiled);
    }
  }
  // ------------------

  // start device
  if (!Rtc.GetIsRunning()) {
    printf("    [INFO] rtc was not actively running, starting now\n");
    Rtc.SetIsRunning(true);
  }

  // check time
  RtcDateTime now = Rtc.GetDateTime();
  if (now < compiled) {
    printf("   [INFO] rtc time older than compile time! Updating DateTime.\n");
    Rtc.SetDateTime(compiled);
  } else if (now > compiled) {
    printf("   [INFO] rtc time newer than compile time.\n");
  } else if (now == compiled) {
    printf("   [INFO] rtc time equal to compile time.\n");
  }

  // set pin
  Rtc.SetSquareWavePin(DS1307SquareWaveOut_Low);

  xSemaphoreGive(i2cBus.mutex);

  set_SleepTime(1000);
  snprintf(msg, 100, "[v] Init 'RTC' inited\n");
  printf(msg);
  driverDisplay.print(msg);
}

RtcDateTime RTC::read_rtc_datetime(void) {
  RtcDateTime now;
  xSemaphoreTakeT(i2cBus.mutex);
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
  now = Rtc.GetDateTime();

  xSemaphoreGive(i2cBus.mutex);

  return now;
}

void RTC::task() {

  while (1) {

    // get date & time
    RtcDateTime now = read_rtc_datetime();
    debug_printf("[RTC] current datetime: %02u/%02u/%04u %02u:%02u:%02u\n", now.Month(), now.Day(), now.Year(), now.Hour(), now.Minute(),
                 now.Second());
    // setTime(30, 24, 15, 17, 1, 2021); // 17th Jan 2021 15:24:30
    esp32time.setTime(now.Second(), now.Minute(), now.Hour(), now.Day(), now.Month(), now.Year()); // 17th Jan 2021 15:24:30
    // sleep for 1s
    vTaskDelay(sleep_polling_ms / portTICK_PERIOD_MS);
  }
}