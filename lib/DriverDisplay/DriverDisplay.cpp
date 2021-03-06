//
// Display
//

#include "../../include/definitions.h"

#include <SPIBus.h>
#include "DriverDisplay.h"

#include <Adafruit_GFX.h>     // graphics library
#include <Adafruit_ILI9341.h> // display

Adafruit_ILI9341 tft = Adafruit_ILI9341(SPI_CS_TFT, SPI_DC, SPI_MOSI, SPI_CLK, SPI_RST, SPI_MISO);

int speedFrameCx;
int speedFrameCy;
int speedFrameX;
int speedFrameY;

char indicatorDirection = 'o';
bool indicatorState = false;

int counterIndicator = 0;
int counterSpeed = 0;

GFXcanvas1 canvas(144, 64); // 128x32 pixel canvas

void init_driver_display(void)
{
    // CRITICAL SECTION SPI: start
    xSemaphoreTake(spi_mutex, portMAX_DELAY);

    // TODO: Display OLED-128-6LGA
    //     SPI.begin(4);
    //     SPI.beginTransaction(SPISettings(1000000, SPI_MSBFIRST, SPI_MODE0));
    //     SPI.transfer(0xF0);
    //     SPI.transfer(0x40);
    //     SPI.transfer(0xA0);
    //     SPI.transfer(0xC0);
    //     SPI.transfer(0xA6);

    //     SPI.transfer(0x81); SPI.transfer(0xFF);

    //     SPI.transfer(0xD5); SPI.transfer(0x40);

    //     SPI.transfer(0xD9); SPI.transfer(0x44);

    //     SPI.transfer(0xAF);
    //     SPI.endTransaction();
    //END - TODO
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
    //tft.setFont(&FreeSans9pt7b);
    tft.setRotation(1);
    tft.setCursor(0, 0);
    tft.setTextColor(ILI9341_GREEN);
    tft.setTextSize(1);
    tft.println("inited.");
    calc_and_draw_display_background();

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

void driver_display_task(void *pvParameter)
{
    // polling loop
    while (1)
    {

        // CRITICAL SECTION SPI: start
        xSemaphoreTake(spi_mutex, portMAX_DELAY);

        unsigned long start;
        start = micros();

        switch (counterIndicator++)
        {
        case 0:
            setIndicatorDirection('w');
            break;
        case 1:
            setIndicatorDirection('l');
            break;
        case 2:
            setIndicatorDirection('r');
            break;
        default:
            setIndicatorDirection('o');
            counterIndicator = 0;
        }

        switch (counterSpeed++)
        {
        case 0:
            draw_info_border(ILI9341_GREEN);
            write_speed(1, ILI9341_YELLOW);
            break;
        case 1:
            write_speed(12, ILI9341_YELLOW);
            break;
        case 2:
            write_speed(123, ILI9341_GREEN);
            break;
        case 3:
            draw_info_border(ILI9341_YELLOW);
            write_speed(888, ILI9341_YELLOW);
            break;
        case 4:
            draw_info_border(ILI9341_RED);
            write_speed(120, ILI9341_RED);
            break;
        case 5:
            write_speed(42, ILI9341_GREEN);
            break;
        case 6:
            write_speed(0, ILI9341_RED);
            draw_info_border(ILI9341_RED);
            break;
        default:
            write_speed(counterSpeed, ILI9341_RED);
            draw_info_border(ILI9341_GREEN);
            if( counterSpeed > 990){
                counterSpeed = 0;
            }
        }

        Serial.printf("time elapsed: %ld\n", micros() - start);

        xSemaphoreGive(spi_mutex);
        // CRITICAL SECTION SPI: end

        // sleep for 1s
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void calc_and_draw_display_background()
{
    int cx = tft.width();
    speedFrameCx = 154;
    speedFrameCy = 70;
    speedFrameX = (cx - speedFrameCx) / 2;
    speedFrameY = 70;

    tft.fillScreen(ILI9341_BLACK);
    draw_info_border(ILI9341_GREEN);
    tft.drawRoundRect(speedFrameX, speedFrameY, speedFrameCx, speedFrameCy, 4, ILI9341_YELLOW);
}

void draw_info_border(int color)
{
    int cx = tft.width();
    int cy = tft.height();
    tft.drawRoundRect(0, 0, cx, cy, 8, color);
}

void write_speed(int speed, int color)
{
    //DOTO_KSC: split in 3 separate digits and draw one by one
    int text_size = 8;
    tft.setTextSize(text_size);
    tft.setTextColor(color);
    tft.setTextSize(text_size);
    tft.fillRoundRect(speedFrameX + 1, speedFrameY + 1, speedFrameCx - 2, speedFrameCy - 2, 4, ILI9341_BLACK);

    // print right adjusted numbers
    if (speed < 10)
    {
        tft.setCursor(speedFrameX + 6 + 2 * text_size * 6, speedFrameY + 6);
    }
    else if (speed < 100)
    {
        tft.setCursor(speedFrameX + 6 + 1 * text_size * 6, speedFrameY + 6);
    }
    else
    {
        tft.setCursor(speedFrameX + 6 + 0 * text_size * 6, speedFrameY + 6);
    }
    tft.println(speed);

    // canvas.setTextSize(text_size);
    // if (speed < 10)
    // {
    //     canvas.setCursor(2 * text_size * 6, 4);
    // }
    // else if (speed < 100)
    // {
    //     canvas.setCursor(1 * text_size * 6, 4);
    // }
    // else
    // {
    //     canvas.setCursor(0 * text_size * 6, 4);
    // }
    // canvas.setCursor(0,0);
    // canvas.fillRect(0, 0, canvas.width(), canvas.height(), ILI9341_BLACK);
    // canvas.setTextColor(color);
    // canvas.println(speed);
    // tft.drawBitmap(speedFrameX + 6, speedFrameY + 4, canvas.getBuffer(), canvas.width(), canvas.height(), color, ILI9341_BLACK); // Copy to screen
    //Clean canvas
    // canvas.setTextColor(ILI9341_BLACK);
    // canvas.println(speed);

    Serial.printf("Speed: %d\n", speed);
}

char getIndicatorDirection()
{
    return indicatorDirection;
}

void setIndicatorDirection(char direction)
{
    indicatorDirection = direction;
    Serial.printf("Indicator: %c\n", direction);
}

bool getIndicatorState()
{
    return indicatorState;
}

void setIndicatorState(bool state)
{
    indicatorState = state;
}

void turn_indicator(char direction)
{
    // l - left
    // r - right
    // o - off
    // w - hazard warning
    int leftX = 10;
    int y = 18;
    int rightX = 300;
    int size = 20;

    switch (direction)
    {
    case 'l':
        tft.fillTriangle(leftX, y, leftX + size, y - size / 2, leftX + size, y + size / 2, ILI9341_YELLOW);
        tft.fillTriangle(rightX, y, rightX - size, y - size / 2, rightX - size, y + size / 2, ILI9341_BLACK);
        break;

    case 'r':
        tft.fillTriangle(leftX, y, leftX + size, y - size / 2, leftX + size, y + size / 2, ILI9341_BLACK);
        tft.fillTriangle(rightX, y, rightX - size, y - size / 2, rightX - size, y + size / 2, ILI9341_YELLOW);
        break;

    case 'o':
        tft.fillTriangle(leftX, y, leftX + size, y - size / 2, leftX + size, y + size / 2, ILI9341_BLACK);
        tft.fillTriangle(rightX, y, rightX - size, y - size / 2, rightX - size, y + size / 2, ILI9341_BLACK);
        break;

    case 'w':
    default:
        tft.fillTriangle(leftX, y, leftX + size, y - size / 2, leftX + size, y + size / 2, ILI9341_YELLOW);
        tft.fillTriangle(rightX, y, rightX - size, y - size / 2, rightX - size, y + size / 2, ILI9341_YELLOW);
    }
}
