//
// Car State Values
//

#ifndef SER_DISPLAYVALUE_H
#define SER_DISPLAYVALUE_H

#include <Adafruit_GFX.h>     // graphics library
#include <Adafruit_ILI9341.h> // display
#include <stdio.h>            /* printf */
#include <stdlib.h>           /* abs */
#include <typeinfo>

#include <Display.h>
#include <LocalFunctionsAndDevices.h>
#include <SPIBus.h>
#include <definitions.h>

extern SPIBus spiBus;

using namespace std;

template <typename T> class DisplayValue {

private:
  T _epsilon;
  string Label;
  string Format;
  string Unit;
  int X;
  int Y;
  int vX;
  int vY;
  int vHeight;
  int vWidth;
  int TextSize;
  int TextColor;
  int BgColor;

public:
  DisplayValue(int x, int y, string label, string format = "%4.1f", string unit = "", int textColor = ILI9341_BLACK,
               int bgColor = ILI9341_ORANGE, int textSize = 2) {
    X = x;
    Y = y;
    Label = label;
    Format = format;
    Unit = unit;
    TextColor = textColor;
    TextSize = textSize;
    BgColor = bgColor;
  }
  virtual ~DisplayValue() {}

  T Value;
  T ValueLast;

  // set the new value and returns the last set value
  T set(T theValue) {
    Value = theValue;
    return ValueLast;
  }

  // get the recent value
  T get() { return Value; }

  // get the last value
  T get_last() { return ValueLast; }

  // makes the recent value to the last value
  void overtake_recent_to_last() { ValueLast = Value; }

  void set_epsilon(T theEpsilon) { _epsilon = theEpsilon; }

  // check if there is a vaue difference between current and last
  bool is_changed() { return abs(Value - ValueLast) > _epsilon; }

  void showLabel(Adafruit_ILI9341 tft) {
    char label[15];
    snprintf(label, 15, "%s", Label.c_str());
    int16_t x1, y1;
    uint16_t w, h;
    xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
    tft.setTextSize(TextSize);
    tft.setTextColor(TextColor);
    tft.setCursor(X, Y);
    tft.print(label);
    tft.getTextBounds(label, X, Y, &x1, &y1, &w, &h);
    tft.printf("    %s", Unit.c_str());
    xSemaphoreGive(spiBus.mutex);
    vX = x1 + w;
    vY = y1;
    vHeight = h;
    //printf(Format.substr(1,1).c_str());
    vWidth = TextSize * 6 * 4; //atoi(Format.substr(1,1).c_str());
  }
  char buffer[20];

  void showValue(Adafruit_ILI9341 tft) { showValue(Value, tft); }

  void showValue(string s, Adafruit_ILI9341 tft) {
    if (s.compare(ValueLast) == 0) {
      _showValue(tft, Value.c_str());
      ValueLast = Value;
    }
  }

  void showValue(bool b, Adafruit_ILI9341 tft) {
    if (b != ValueLast) {
      _showValue(tft, Value ? "ON" : "OFF");
      ValueLast = Value;
    }
  }

  void showValue(int s, Adafruit_ILI9341 tft) {
    if (is_changed()) {
      snprintf(buffer, 20, Format.c_str(), Value);
      _showValue(tft, buffer);
      ValueLast = Value;
    }
  }

  void showValue(float s, Adafruit_ILI9341 tft) {
    if (is_changed()) {
      snprintf(buffer, 20, Format.c_str(), Value);
      _showValue(tft, buffer);
      ValueLast = Value;
    }
  }

  void _showValue(Adafruit_ILI9341 tft, const char *value) {
    int valX = X + Label.length() * TextSize * 6;
    xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
    tft.setTextSize(TextSize);
    tft.setTextColor(TextColor);
    tft.fillRect(vX, vY, vWidth, vHeight, BgColor);
    tft.setCursor(valX, Y);
    tft.print(value);
    xSemaphoreGive(spiBus.mutex);
  }
};

#endif // SER_DISPLAYVALUE_H
