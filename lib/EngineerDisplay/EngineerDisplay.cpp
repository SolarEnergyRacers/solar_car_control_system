//
// Display
//

#include <definitions.h>

#include <Adafruit_GFX.h>     // graphics library
#include <Adafruit_SSD1305.h> // display

#include <I2CBus.h>

#include <EngineerDisplay.h>

extern I2CBus i2cBus;
Adafruit_SSD1305 display(OLED_WIDTH, OLED_HEIGHT, &Wire, OLED_RESET); // TODO: remove & add to class

void EngineerDisplay::re_init() { init(); }

void EngineerDisplay::init(void) {

  // CRITICAL SECTION I2C: start
  xSemaphoreTake(i2cBus.mutex, portMAX_DELAY);

  if (!display.begin(I2C_ADDRESS_SSD1305)) {
    printf("[Display] Unable to initialize OLED screen.\n");
  } else {
    printf("[Display] Screen initialize successfully.\n");
  }

  // init done
  display.display(); // show splashscreen

  xSemaphoreGive(i2cBus.mutex);

  vTaskDelay(1000 / portTICK_PERIOD_MS);

  xSemaphoreTake(i2cBus.mutex, portMAX_DELAY);

  display.clearDisplay(); // clears the screen and buffer

  xSemaphoreGive(i2cBus.mutex);
}

void EngineerDisplay::exit() {
  // TODO
}

void EngineerDisplay::task() {

  // polling loop
  while (1) {
    xSemaphoreTake(i2cBus.mutex, portMAX_DELAY);

    // clears the screen and buffer
    display.clearDisplay();

    // setup params
    display.setTextSize(1);
    display.setTextWrap(false);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);

    // print demo characters
    for (int i = 0; i < 168; i++) {
      if (i == '\n')
        continue;
      // write char
      display.write(i);
      // newline
      if ((i > 0) && (i % 21 == 0))
        display.println();
    }
    display.display();

    xSemaphoreGive(i2cBus.mutex);
    // sleep for 1s
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}