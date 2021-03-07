//
// Display
//

#include "../../include/definitions.h"

#include <SPIBus.h>
#include "DriverDisplay.h"

#include <Adafruit_GFX.h>     // graphics library
#include <Adafruit_ILI9341.h> // display

Adafruit_ILI9341 tft = Adafruit_ILI9341(SPI_CS_TFT, SPI_DC, SPI_MOSI, SPI_CLK, SPI_RST, SPI_MISO);

// display formats and sizes
int infoFrameX = 0;
int infoFrameY = 0;
int infoFrameSizeX = 0; // full tft width
int infoFrameSizeY = 16;
int infoTextSize = 3;

int bigFrameX = 64;

int speedFrameX = 0; //(sizeX - speedFrameCx) / 2;
int speedFrameY = 80;
int speedFrameSizeX = 156;
int speedFrameSizeY = 76;
int speedTextSize = 8;

int batFrameX = 10;
int batFrameY = 180;
int batTextSize = 2;

int pvFrameX = 10;
int pvFrameY = 200;
int pvTextSize = 2;

int motorFrameX = 10;
int motorFrameY = 220;
int motorTextSize = 2;

int indicatorLeftX = 10;
int indicatorY = 100;
int indicatorRightX = 300;
int indicatorWidth = 30;
int indicatorHeight = 20;

// dsiplay cache
char indicatorDirection = 'o';
bool indicatorState = false;
String infoLast = "";
int speedLast = -1;
float batLast = -1;
float pvLast = -1;
float motorLast = -1;

// counters for demo display
int counterIndicator = 0;
int counterSpeed = 0;
int counterPV = 0;
// ----------------------------------------------

// writes float value  in the range from -9999.9 to 999.9
float _write_float(int x, int y, float valueLast, float value, int textSize, int color)
{
    if (value < -9999.9 || value > 9999.9)
    {
        Serial.printf("ERROR: call _write_float with a value outside the range: '%f'\n", value);
        return value;
    }
    tft.setTextSize(textSize);
    tft.setTextColor(color);
    tft.setCursor(x, y);
    int digitWidth = textSize * 6;
    int digitHeight = textSize * 8;
    //tft.fillRect(x, y, 6 * digitWidth, 1 * digitHeight, ILI9341_BLACK);
    // tft.printf("%.1f", value);

    char sign = value < 0 ? '-' : '+';
    char signOld = valueLast < 0 ? '-' : '+';
    float val = abs(value);
    float valOld = abs(valueLast);

    int d1 = (int)val % 10;
    int d2 = ((int)val / 10) % 10;
    int d3 = ((int)val / 100) % 10;
    int d4 = ((int)val / 1000) % 10;
    int d0 = (val - (int)val) * 10;

    int d1o = (int)valOld % 10;
    int d2o = ((int)valOld / 10) % 10;
    int d3o = ((int)valOld / 100) % 10;
    int d4o = ((int)valOld / 1000) % 10;
    int d0o = (valOld - (int)valOld) * 10;

    // Serial.printf("FLOAT: %8.1f [%c|%d-%d-%d-%d-%d] -> %8.1f [%c|%d-%d-%d-%d-%d]\n",
    //               value, sign, d4, d3, d2, d1, d0,
    //               valueLast, signOld, d4o, d3o, d2o, d1o, d0o);

    if (d0 != d0o)
    {
        tft.fillRect(x + (digitWidth + 1) * 5, y, digitWidth * 2, digitHeight, ILI9341_BLACK);
        tft.setCursor(x + (digitWidth + 1) * 5, y);
        tft.printf(".%d", d0);
    }
    if (d1 != d1o)
    {
        tft.fillRect(x + (digitWidth + 1) * 4, y, digitWidth, digitHeight, ILI9341_BLACK);
        tft.setCursor(x + (digitWidth + 1) * 4, y);
        tft.print(d1);
    }
    if (d2 != d2o)
    {
        tft.fillRect(x + (digitWidth + 1) * 3, y, digitWidth, digitHeight, ILI9341_BLACK);
        tft.setCursor(x + (digitWidth + 1) * 3, y);
        if (abs(value) > 9)
        {
            tft.print(d2);
        }
    }
    if (d3 != d3o)
    {
        tft.fillRect(x + (digitWidth + 1) * 2, y, digitWidth, digitHeight, ILI9341_BLACK);
        tft.setCursor(x + (digitWidth + 1) * 2, y);
        if (abs(value) > 99)
        {
            tft.print(d3);
        }
    }
    if (d4 != d4o)
    {
        tft.fillRect(x + (digitWidth + 1) * 1, y, digitWidth, digitHeight, ILI9341_BLACK);
        tft.setCursor(x + (digitWidth + 1) * 1, y);
        if (abs(value) > 999)
        {
            tft.print(d4);
        }
    }
    if (sign != signOld)
    {
        tft.fillRect(x, y, (digitWidth + 1), digitHeight, ILI9341_BLACK);
        tft.setCursor(x, y);
        tft.print(sign);
    }

    return value;
}

// writes integer value in the range from 0 to 999
int _write_int(int x, int y, int valueLast, int value, int textSize, int color)
{
    if (value < 0 || value > 999)
    {
        Serial.printf("ERROR: call _write_int with a value outside the range: '%d'", value);
        return value;
    }

    tft.setTextSize(textSize);
    tft.setTextColor(color);
    tft.setCursor(x, y);
    int digitWidth = textSize * 6;
    int digitHeight = textSize * 8;

    int d1 = (int)value % 10;
    int d2 = ((int)value / 10) % 10;
    int d3 = ((int)value / 100) % 10;

    int d1o = (int)valueLast % 10;
    int d2o = ((int)valueLast / 10) % 10;
    int d3o = ((int)valueLast / 100) % 10;

    if (d1 != d1o)
    {
        tft.fillRect(x + (digitWidth + 1) * 2, y, digitWidth, digitHeight, ILI9341_BLACK);
        tft.setCursor(x + (digitWidth + 1) * 2, y);
        tft.print(d1);
    }
    if (d2 != d2o)
    {
        tft.fillRect(x + (digitWidth + 1) * 1, y, digitWidth, digitHeight, ILI9341_BLACK);
        tft.setCursor(x + (digitWidth + 1) * 1, y);
        if (abs(value) > 9)
        {
            tft.print(d2);
        }
    }
    if (d3 != d3o)
    {
        tft.fillRect(x + (digitWidth + 1) * 0, y, digitWidth, digitHeight, ILI9341_BLACK);
        tft.setCursor(x + (digitWidth + 1) * 0, y);
        if (abs(value) > 99)
        {
            tft.print(d3);
        }
    }

    return value;
}

void draw_display_border(int color)
{
    tft.drawRoundRect(0, bigFrameX, tft.width(), tft.height() - bigFrameX, 8, color);
}

void draw_speed_border(int color)
{
    speedFrameX = (tft.width() - speedFrameSizeX) / 2;
    tft.drawRoundRect(speedFrameX, speedFrameY, speedFrameSizeX, speedFrameSizeY, 4, color);
}

void draw_display_background()
{
    tft.fillScreen(ILI9341_BLACK);
    draw_display_border(ILI9341_GREEN);
    draw_speed_border(ILI9341_YELLOW);

    tft.setTextSize(2);
    tft.setTextColor(ILI9341_DARKGREEN);
    tft.setCursor(batFrameX, batFrameY);
    tft.print("  Bat(V):");

    tft.setCursor(pvFrameX, pvFrameY);
    tft.print("   PV(A):");

    tft.setCursor(motorFrameX, motorFrameY);
    tft.print("Motor(A):");
}

void write_speed(int speed, int color)
{
    speedLast = _write_int(speedFrameX + 9, speedFrameY + 10, speedLast, speed, speedTextSize, ILI9341_GREEN);
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

void show_Faster(bool on)
{
    int x = speedFrameX;
    int y = speedFrameY - 3;
    int color;
    if (on)
    {
        show_Slower(false);
        color = ILI9341_YELLOW;
    }
    else
    {
        color = ILI9341_BLACK;
    }
    tft.fillTriangle(x, y, x + speedFrameSizeX, y, x + speedFrameSizeX / 2, y - 10, color);
}

void show_Slower(bool on)
{
    int x = speedFrameX;
    int y = speedFrameY + speedFrameSizeY + 3;
    int color;
    if (on)
    {
        show_Faster(false);
        color = ILI9341_RED;
    }
    else
    {
        color = ILI9341_BLACK;
    }
    tft.fillTriangle(x, y, x + speedFrameSizeX, y, x + speedFrameSizeX / 2, y + 10, color);
}

void _turn_Left(int color)
{
    int x = indicatorLeftX;
    int y = indicatorY;
    tft.fillTriangle(x, y, x + indicatorWidth, y - indicatorHeight, x + indicatorWidth, y + indicatorHeight, color);
}

void _turn_Right(int color)
{
    int x = indicatorRightX;
    int y = indicatorY;
    tft.fillTriangle(x, y, x - indicatorWidth, y - indicatorHeight, x - indicatorWidth, y + indicatorHeight, color);
}

void turn_indicator(char direction)
{
    // l - left
    // r - right
    // o - off
    // w - hazard warning
    _turn_Left(ILI9341_BLACK);
    _turn_Right(ILI9341_BLACK);
    switch (direction)
    {
    case 'l':
        _turn_Left(ILI9341_YELLOW);
        break;

    case 'r':
        _turn_Right(ILI9341_YELLOW);
        break;

    case 'o':
        break;

    case 'w':
    default:
        _turn_Left(ILI9341_RED);
        _turn_Right(ILI9341_RED);
    }
}

void write_bat(float value, int color)
{
    int labelLen = 9;
    int labelOffset = labelLen * batTextSize * 6;
    batLast = _write_float(batFrameX + labelOffset, batFrameY, batLast, value, batTextSize, color);
}

void write_pv(float value, int color)
{
    int labelLen = 9;
    int labelOffset = labelLen * pvTextSize * 6;
    pvLast = _write_float(pvFrameX + labelOffset, pvFrameY, pvLast, value, pvTextSize, color);
}

void write_motor(float value, int color)
{
    int labelLen = 9;
    int labelOffset = labelLen * motorTextSize * 6;
    motorLast = _write_float(motorFrameX + labelOffset, motorFrameY, motorLast, value, motorTextSize, color);
}

void _drawCentreString(const String &buf, int x, int y)
{
    int16_t x1, y1;
    uint16_t w, h;
    tft.getTextBounds(buf, x, y, &x1, &y1, &w, &h); //calc width of new string
    tft.setCursor(x - w / 2, y);
    tft.print(buf);
}

void write_driver_info(String msg, int color)
{
    int textSize = 4;

    if (msg != infoLast)
    {
        //Serial.printf("delete '%s', write '%s'", infoLast.c_str(), msg.c_str());
        // tft.fillRect(infoFrameX, infoFrameY, infoFrameSizeX, infoFrameSizeY, ILI9341_BLACK);
        tft.setTextSize(textSize);
        tft.setTextWrap(true);

        tft.setTextColor(ILI9341_BLACK);
        tft.setCursor(infoFrameX, infoFrameY);
        tft.print(infoLast);

        tft.setTextColor(color);
        tft.setCursor(infoFrameX, infoFrameY);
        tft.print(msg);
        //_drawCentreString(msg.c_str(), 0, 0);
        infoLast = msg;
    }
}
// ------------------
// FreeRTOS INIT TASK
// ------------------
void init_driver_display(void)
{
    // CRITICAL SECTION SPI: start
    xSemaphoreTake(spi_mutex, portMAX_DELAY);

#ifdef DISPLAY_OLED_128_6LGA
    // TODO: DISPLAY_OLED_128_6LGA
    Serial.println("Display Test DISPLAY_OLED_128_6LGA");
    SPI.begin(4);
    SPI.beginTransaction(SPISettings(1000000, SPI_MSBFIRST, SPI_MODE0));
    SPI.transfer(0xF0);
    SPI.transfer(0x40);
    SPI.transfer(0xA0);
    SPI.transfer(0xC0);
    SPI.transfer(0xA6);

    SPI.transfer(0x81);
    SPI.transfer(0xFF);

    SPI.transfer(0xD5);
    SPI.transfer(0x40);

    SPI.transfer(0xD9);
    SPI.transfer(0x44);

    SPI.transfer(0xAF);
    SPI.endTransaction();
    Serial.println("End Display Test DISPLAY_OLED_128_6LGA");
#endif

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
    infoFrameSizeX = tft.width();

    //tft.setFont(&FreeSans9pt7b);
    tft.setRotation(1);
    draw_display_background();

    write_driver_info("0123456789ABCDEF0123456789", ILI9341_WHITE);
    write_speed(888, ILI9341_GREEN);
    write_bat(-8888.8, ILI9341_GREEN);
    write_pv(-8888.8, ILI9341_GREEN);
    write_motor(-8888.8, ILI9341_GREEN);
    show_Faster(true);
    setIndicatorDirection('w');
    delay(500);
    show_Slower(true);
    delay(500);
    setIndicatorDirection('o');
    write_driver_info("", ILI9341_WHITE);
    write_speed(0, ILI9341_GREEN);
    write_bat(0, ILI9341_GREEN);
    write_pv(0, ILI9341_GREEN);
    write_motor(0, ILI9341_GREEN);

    xSemaphoreGive(spi_mutex);
    // CRITICAL SECTION SPI: end

    vTaskDelay(1000 / portTICK_PERIOD_MS);
}

// -------------
// FreeRTOS TASK
// -------------
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

        //unsigned long start;
        //start = micros();

        switch (counterIndicator++)
        {
        case 0:
            setIndicatorDirection('w');
            write_driver_info("Stop!", ILI9341_RED);
            show_Slower(false);
            show_Faster(false);
            break;
        case 40:
            setIndicatorDirection('l');
            write_driver_info("Go", ILI9341_WHITE);
            //write_driver_info("0123456789ABCDEF0123456789", ILI9341_WHITE);
            show_Faster(true);
            break;
        case 80:
            setIndicatorDirection('r');
            write_driver_info("Go", ILI9341_WHITE);
            show_Slower(true);
            break;
        case 120:
            setIndicatorDirection('o');
            write_driver_info("", ILI9341_WHITE);
            show_Slower(false);
            show_Faster(false);
            break;
        case 170:
            counterIndicator = 0;
        }

        switch (counterSpeed++)
        {
        case 0:
            draw_display_border(ILI9341_GREEN);
            write_speed(1, ILI9341_YELLOW);
            break;
        case 1:
            write_speed(12, ILI9341_YELLOW);
            break;
        case 2:
            write_speed(123, ILI9341_GREEN);
            break;
        case 3:
            draw_display_border(ILI9341_YELLOW);
            write_speed(888, ILI9341_YELLOW);
            delay(1000);
            break;
        case 4:
            draw_display_border(ILI9341_RED);
            write_speed(120, ILI9341_RED);
            break;
        case 5:
            write_speed(42, ILI9341_GREEN);
            break;
        case 6:
            write_speed(0, ILI9341_RED);
            draw_display_border(ILI9341_RED);
            break;
        default:
            write_speed(counterSpeed, ILI9341_GREENYELLOW);
            draw_display_border(ILI9341_GREEN);
            if (counterSpeed > 990)
            {
                counterSpeed = 0;
            }
        }

        switch (counterPV++)
        {
        case 0:
            write_pv(1, ILI9341_YELLOW);
            write_motor(1.1, ILI9341_YELLOW);
            break;
        case 1:
            write_pv(12, ILI9341_YELLOW);
            write_motor(12.3, ILI9341_YELLOW);
            break;
        case 2:
            write_pv(123, ILI9341_YELLOW);
            write_motor(123.4, ILI9341_YELLOW);
            break;
        case 3:
            write_pv(1234, ILI9341_YELLOW);
            write_motor(1234.5, ILI9341_YELLOW);
            break;
        case 4:
            write_pv(-1, ILI9341_YELLOW);
            write_motor(-1.2, ILI9341_YELLOW);
            break;
        case 5:
            write_pv(-12, ILI9341_YELLOW);
            write_motor(-12.3, ILI9341_YELLOW);
            break;
        case 6:
            write_pv(-123, ILI9341_YELLOW);
            write_motor(-123.4, ILI9341_YELLOW);
            counterPV = -99999;
            break;
        default:
            write_pv((float)counterPV / 10, ILI9341_YELLOW);
            write_motor((float)counterPV / -10, ILI9341_YELLOW);
            write_bat((float)counterPV / 1000, ILI9341_BLUE);
            if (counterPV > 99999)
            {
                counterPV = -99999;
            }
        }

        //Serial.printf("time elapsed: %ld\n", micros() - start);

        xSemaphoreGive(spi_mutex);
        // CRITICAL SECTION SPI: end

        // sleep for 1s
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
