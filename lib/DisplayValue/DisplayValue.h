//
// Car State Values
//

#ifndef SER_DISPLAYVALUE_H
#define SER_DISPLAYVALUE_H

#include <LocalFunctionsAndDevices.h>
#include <definitions.h>

#include <Adafruit_GFX.h>     // graphics library
#include <Adafruit_ILI9341.h> // display
#include <stdio.h>            /* printf */
#include <stdlib.h>           /* abs */
#include <typeinfo>

#include <Helper.h>
#include <SPIBus.h>

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
  bool IsInited;

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
    IsInited = false;
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
  T get_recent_overtake_last() {
    ValueLast = Value;
    return Value;
  }

  void change_format(string fmt) { Format = fmt; }

  void set_epsilon(T theEpsilon) { _epsilon = theEpsilon; }

  // check if there is a vaue difference between current and last
  bool is_changed() { return abs(Value - ValueLast) > _epsilon; }

  void showLabel(Adafruit_ILI9341 tft) {
    char label[15];
    snprintf(label, 15, "%s", Label.c_str());
    int16_t x1, y1;
    uint16_t w, h;
    vWidth = atoi(Format.substr(1, 1).c_str()) * TextSize * 6 + 4;
    xSemaphoreTakeT(spiBus.mutex);
    tft.setTextSize(TextSize);
    tft.setTextColor(TextColor);
    tft.setCursor(X, Y);
    tft.print(label);
    tft.getTextBounds(label, X, Y, &x1, &y1, &w, &h);
    if (h == 0)
      h = TextSize * 8;
    tft.setCursor(X + w + vWidth, Y);
    tft.printf("%s", Unit.c_str());
    xSemaphoreGive(spiBus.mutex);
    vX = x1 + w;
    vY = y1;
    vHeight = h;
    IsInited = false; // showValue must be done for every Value
  }
  char buffer[20];

  void showValue(Adafruit_ILI9341 tft) { showValue(Value, tft); }

  void showValue(string s, Adafruit_ILI9341 tft) {
    if (s.compare(ValueLast) == 0 || !IsInited) {
      _showValue(tft, Value.c_str());
    }
  }

  void showValue(bool b, Adafruit_ILI9341 tft) {
    if (b != ValueLast || !IsInited) {
      _showValue(tft, Value ? "ON" : "OFF");
    }
  }

  void showValue(int s, Adafruit_ILI9341 tft) {
    if (is_changed() || !IsInited) {
      snprintf(buffer, 20, Format.c_str(), Value);
      _showValue(tft, buffer);
    }
  }

  void showValue(float s, Adafruit_ILI9341 tft) {
    if (is_changed() || !IsInited) {
      snprintf(buffer, 20, Format.c_str(), Value);
      _showValue(tft, buffer);
    }
  }

  void _showValue(Adafruit_ILI9341 tft, const char *value) {
    xSemaphoreTakeT(spiBus.mutex);
    tft.setTextSize(TextSize);
    tft.setTextColor(TextColor);
    tft.fillRect(vX, vY, vWidth, vHeight, BgColor);
    tft.setCursor(vX, vY);
    tft.print(value);
    xSemaphoreGive(spiBus.mutex);

    ValueLast = Value;
    IsInited = true;
  }
};

#endif // SER_DISPLAYVALUE_H
