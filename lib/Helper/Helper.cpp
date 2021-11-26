//
// Helper Functions
//
#include <definitions.h>

#include <iostream>
#include <stdio.h>
#include <string>

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

#include <Helper.h>

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
    printf("[%d:%d]", i, c);
  } while (i < (n - 1) && c != '\n' && c != '\r');
  str[i] = 0; // add zero-termination

  // on success
  return str;
}

void xSemaphoreTakeT(QueueHandle_t mutex) {
  if (!xSemaphoreTake(mutex, portMAX_DELAY)) {
    cout << "ERROR: mutex ************************************ " << mutex << " ****************" << endl;
    throw runtime_error("ERROR: mutex");
  }
}
