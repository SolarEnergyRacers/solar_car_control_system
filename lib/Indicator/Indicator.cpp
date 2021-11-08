//
// Display
//

#include <definitions.h>

#include <Display.h>
#include <DriverDisplay.h>
#include <IOExt.h>
#include <Indicator.h>
#include <PCF8574.h>

extern CarState carState;
extern DriverDisplay driverDisplay;
extern IOExt ioExt;
extern Indicator indicator;

// ------------------
// FreeRTOS functions
string Indicator::getName(void) { return "Indicator"; };

void Indicator::re_init() { init(); }

void Indicator::init(void) {
  set_SleepTime(500);
  printf("[v] Indicator handler inited\n");
  driverDisplay.print("[v] " + getName() + " initialized.\n");
}

void Indicator::exit(void){
    // TODO
};
// ------------------

void Indicator::setIndicator(INDICATOR state) {
  if (carState.Indicator.get() == state) {
    debug_printf("Set indicator '%d' off\n", static_cast<int>(state));
    carState.Indicator.set(INDICATOR::OFF);
  } else {
    debug_printf("Set indicator '%d' on\n", static_cast<int>(state));
    carState.Indicator.set(state);
  }
}

// pins are low active
void Indicator::setIndicator(int left, int right) {
  if (left == 0 && right == 0)
    setIndicator(INDICATOR::WARN);
  else if (left == 0)
    setIndicator(INDICATOR::LEFT);
  else if (right == 0)
    setIndicator(INDICATOR::RIGHT);
}

bool Indicator::getIndicatorLeft() {
  return (carState.Indicator.get() == INDICATOR::LEFT || carState.Indicator.get() == INDICATOR::WARN) && carState.IndicatorBlink.get();
}

bool Indicator::getIndicatorRight() {
  return (carState.Indicator.get() == INDICATOR::RIGHT || carState.Indicator.get() == INDICATOR::WARN) && carState.IndicatorBlink.get();
}

unsigned long lastFlip = 0;
void Indicator::task() {
  // do not add code here -- only controlling the blink frequency
  // polling loop
  while (1) {
    if (carState.Indicator.get() != INDICATOR::OFF) {
      if (carState.IndicatorBlink.get() && (millis() - lastFlip) > intervall_on) {
        lastFlip = millis();
        carState.IndicatorBlink.set(false);
      } else if (!carState.IndicatorBlink.get() && (millis() - lastFlip) > intervall_off) {
        lastFlip = millis();
        carState.IndicatorBlink.set(true);
      }
    }
    ioExt.setPort(carState.getPin(PinIndicatorOutLeft)->port, indicator.getIndicatorLeft());
    ioExt.setPort(carState.getPin(PinIndicatorOutRight)->port, indicator.getIndicatorRight());

    // sleep
    vTaskDelay(sleep_polling_ms / portTICK_PERIOD_MS);
  }
}
