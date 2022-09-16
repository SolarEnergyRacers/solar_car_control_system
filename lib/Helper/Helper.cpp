//
// Helper Functions
//
#include <definitions.h>

#include <fmt/core.h>
#include <iostream>
#include <string>

#include <RTC.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

#include <Console.h>
#include <Helper.h>

extern Console console;
using namespace std;

char *fgets_stdio_blocking(char *str, int n) {

  char c;
  int i = 0;
  do {
    c = getchar();
    if (c == 255) { // no char available to consume
      // taskYIELD(); // does not seem to do the trick
      vTaskDelay(100 / portTICK_PERIOD_MS);
    } else { // store char
      str[i++] = c;
    }
    console << "[" << i << ":" << c << "]";
  } while (i < (n - 1) && c != '\n' && c != '\r');
  str[i] = 0; // add zero-termination

  // on success
  return str;
}

void xSemaphoreTakeT(xQueueHandle mutex) {
  if (!xSemaphoreTake(mutex, portMAX_DELAY)) {
    console << "ERROR: mutex ************************************ " << mutex << " ****************\n";
    throw runtime_error("ERROR: mutex");
  }
}

string getDateTime() {
  time_t nowDateTime = time(NULL);
  struct tm t = *localtime(&nowDateTime);   
  return fmt::format("{:04d}-{:02d}-{:02d},{:02d}:{:02d}:{:02d}", t.tm_year, t.tm_mon, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);;
}

string formatDateTime(RtcDateTime now) {
  string static dateTimeString =
      fmt::format("{:04d}-{:02d}-{:02d},{:02d}:{:02d}:{:02d}", now.Year(), now.Month(), now.Day(), now.Hour(), now.Minute(), now.Second());
  return dateTimeString;
}

string getTimeStamp() {
  unsigned long seconds = millis() / 1000;
  unsigned long secsRemaining = seconds % 3600;
  int runHours = seconds / 3600;
  int runMinutes = secsRemaining / 60;
  int runSeconds = secsRemaining % 60;
  return fmt::format("T{:02d}:{:02d}:{:02d}", runHours, runMinutes, runSeconds);
}
