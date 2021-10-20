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

public:
  DisplayValue(int x, int y, string label, string unit, int textColor = ILI9341_BLACK, int textSize = 2) {
    X = x;
    Y = y;
    Label = label;
    Unit = unit;
    TextColor = textColor;
    TextSize = textSize;
  }
  virtual ~DisplayValue() {}

  T Value;
  T ValueLast;
  int X;
  int Y;
  int TextSize;
  int TextColor;
  string Label;
  string Unit;

  // set the new value and returns the last set value
  T set(T theValue) {
    Value = theValue;
    return ValueLast;
  }
  // get the recent value
  T get() { return Value; }
  // get the last value
  T get_last() { return ValueLast; }
  // makes the recent value to the valueLast
  void overtake_recent_to_last() { ValueLast = Value; }
  void set_epsilon(T theEpsilon) { _epsilon = theEpsilon; }
  // check if there is a vaue difference bettween current and last
  bool is_changed() { return abs(Value - ValueLast) > _epsilon; }

  void showLabel(Adafruit_ILI9341 tft) {
    xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
    tft.setTextSize(TextSize);
    tft.setTextColor(TextColor);
    tft.setCursor(X, Y);
    tft.printf("%-7s: --%s", Label.c_str(), Unit.c_str());
    xSemaphoreGive(spiBus.mutex);
  }

  void showValue(Adafruit_ILI9341 tft) {
    xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
    tft.setTextSize(TextSize);
    tft.setTextColor(TextColor);
    int valX = X + 9 * TextSize * 6;
    int valY = Y;
    int digitWidth = TextSize * 6;
    int digitHeight = TextSize * 8;
    tft.fillRect(valX, valY, digitWidth * 3, digitHeight, ILI9341_LIGHTGREY);
    tft.setCursor(valX, Y);
    tft.printf("%s", _getStringValue(Value));
    xSemaphoreGive(spiBus.mutex);
  }

  char *_getStringValue(string val) { return val.c_str(); }
  char *_getStringValue(int val) {
    char buffer[20];
    snprintf(buffer, 20, "%d", val);
    return buffer;
  }
  char *_getStringValue(float val) {
    char buffer[20];
    snprintf(buffer, 20, "%f", val);
    return buffer;
  }
  char *_getStringValue(bool val) {
    if (val)
      return "ON";
    else
      return "OFF";
  }
};

#endif // SER_DISPLAYVALUE_H
