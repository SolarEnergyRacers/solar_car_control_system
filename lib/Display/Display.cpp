//
// Display
//

#include "../../include/definitions.h"

#include <I2CBus.h>

#include "Display.h"

#include <Adafruit_GFX.h> // graphics library
#include <Adafruit_SSD1305.h> // display
#define OLED_RESET 9
Adafruit_SSD1305 display(128, 64, &Wire, OLED_RESET);



void init_display(void){

    // CRITICAL SECTION I2C: start
    xSemaphoreTake(i2c_mutex, portMAX_DELAY);

    if (!display.begin(I2C_ADDRESS_SSD1305) ) {
        printf("[Display] Unable to initialize OLED screen.\n");
    } else {
        printf("[Display] Screen initialize successfully.\n");
    }

    // init done
    display.display(); // show splashscreen

    xSemaphoreGive(i2c_mutex);
    // CRITICAL SECTION I2C: end


    vTaskDelay(1000 / portTICK_PERIOD_MS);

    // CRITICAL SECTION I2C: start
    xSemaphoreTake(i2c_mutex, portMAX_DELAY);

    display.clearDisplay();   // clears the screen and buffer

    xSemaphoreGive(i2c_mutex);
    // CRITICAL SECTION I2C: end
}


void draw_display_demo_task(void *pvParameter){

    // polling loop
    while(1){

        // CRITICAL SECTION I2C: start
        xSemaphoreTake(i2c_mutex, portMAX_DELAY);

        // clears the screen and buffer
        display.clearDisplay();

        // setup params
        display.setTextSize(1);
        display.setTextWrap(false);
        display.setTextColor(WHITE);
        display.setCursor(0,0);

        // print demo characters
        for (int i = 0; i < 168; i++) {
            if (i == '\n') continue;
            // write char
            display.write(i);
            // newline
            if ((i > 0) && (i % 21 == 0))
                display.println();
        }
        display.display();

        xSemaphoreGive(i2c_mutex);
        // CRITICAL SECTION I2C: end

        // sleep for 1s
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
