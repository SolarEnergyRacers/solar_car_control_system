//
// Display
//

#include "../../include/definitions.h"


#include <SPIBus.h>
#include "DisplayLarge.h"

#include <Adafruit_GFX.h>     // graphics library
#include <Adafruit_ILI9341.h> // display

// For the Adafruit shield, these are the default.
#define TFT_DC 12
#define TFT_CS 5
#define TFT_MOSI 23
#define TFT_CLK 18
#define TFT_RST 17
#define TFT_MISO 21
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

void init_display_large(void)
{

    // CRITICAL SECTION SPI: start
    xSemaphoreTake(spi_mutex, portMAX_DELAY);

    tft.begin();
    try
    {
        uint8_t x = tft.readcommand8(ILI9341_RDMODE);
        Serial.print("Display Power Mode: 0x");
        Serial.println(x, HEX);
        x = tft.readcommand8(ILI9341_RDMADCTL);
        Serial.print("MADCTL Mode: 0x");
        Serial.println(x, HEX);
        x = tft.readcommand8(ILI9341_RDPIXFMT);
        Serial.print("Pixel Format: 0x");
        Serial.println(x, HEX);
        x = tft.readcommand8(ILI9341_RDIMGFMT);
        Serial.print("Image Format: 0x");
        Serial.println(x, HEX);
        x = tft.readcommand8(ILI9341_RDSELFDIAG);
        Serial.print("Self Diagnostic: 0x");
        Serial.println(x, HEX);
        printf("[Display Large] Screen initialize successfully.\n");
    }
    catch (__exception ex)
    {
        printf("[Display Large] Unable to initialize OLED screen.\n");
    }

    tft.setRotation(1);
    tft.setCursor(0, 0);
    tft.setTextColor(ILI9341_GREEN);
    tft.setTextSize(1);
    tft.println("initied.");

    xSemaphoreGive(spi_mutex);
    // CRITICAL SECTION SPI: end

    vTaskDelay(1000 / portTICK_PERIOD_MS);
}

void draw_display_large_demo_task(void *pvParameter)
{

    // polling loop
    while (1)
    {

        // CRITICAL SECTION SPI: start
        xSemaphoreTake(spi_mutex, portMAX_DELAY);

        tft.setRotation(1);
        tft.setCursor(0, 0);
        tft.setTextColor(ILI9341_WHITE);
        tft.setTextSize(1);
        tft.println("Hello ILI!");

        // clears the screen and buffer
        tft.fillScreen(ILI9341_BLACK);

        // setup params
        tft.setTextSize(1);
        tft.setTextWrap(false);
        tft.setTextColor(ILI9341_GREENYELLOW);
        tft.setCursor(0, 0);

        // print demo characters
        for (int i = 0; i < 168; i++)
        {
            if (i == '\n')
                continue;
            // write char
            tft.write(i);
            // newline
            if ((i > 0) && (i % 21 == 0))
                tft.println();
        }
        //tft.display();

        xSemaphoreGive(spi_mutex);
        // CRITICAL SECTION SPI: end

        // sleep for 1s
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
