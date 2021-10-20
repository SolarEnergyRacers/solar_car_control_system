//
// Car State Values
//

#ifndef SER_DISPLAYVALUE_H
#define SER_DISPLAYVALUE_H

#include <Adafruit_GFX.h>     // graphics library
#include <Adafruit_ILI9341.h> // display
#include <stdio.h>            /* printf */
#include <stdlib.h>           /* abs */

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

  void set_epsilon(T theEpsilon) { _epsilon = theEpsilon; }
  // check if there is a vaue difference bettween current and last
  bool is_changed() { return abs(Value - ValueLast) > _epsilon; }

  void write(Adafruit_ILI9341 tft) {
    xSemaphoreTake(spiBus.mutex, portMAX_DELAY);
    tft.setTextSize(TextSize);
    tft.setTextColor(TextColor);
    tft.setCursor(X, Y);
    tft.printf("%-7s: --%s", Label.c_str(), Unit.c_str());
    xSemaphoreGive(spiBus.mutex);
  }
};

#endif // SER_DISPLAYVALUE_H
