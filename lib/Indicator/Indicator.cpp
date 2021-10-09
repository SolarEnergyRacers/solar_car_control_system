//
// Display
//

#include <DriverDisplayC.h>
#include <Indicator.h>
#include <definitions.h>

extern CarState carState;

INDICATOR Indicator::getIndicator() { return carState.Indicator.get(); }

void Indicator::setIndicator(INDICATOR state) {
  if (carState.Indicator.get() == state) {
    debug_printf("Set indicator '%d' off\n", static_cast<int>(state));
    carState.Indicator.set(INDICATOR::OFF);
  } else {
    debug_printf("Set indicator '%d' on\n", static_cast<int>(state));
    carState.Indicator.set(state);
  }
}

void Indicator::setIndicator(int left, int right) {
  if (left == 0 && right == 0)
    setIndicator(INDICATOR::WARN);
  else if (left == 0)
    setIndicator(INDICATOR::LEFT);
  else if (right == 0)
    setIndicator(INDICATOR::RIGHT);
}

void Indicator::re_init() { init(); }

string Indicator::getName(void) { return "Indicator"; };

void Indicator::exit(void){
    // TODO
};

// ------------------
// FreeRTOS INIT TASK
// ------------------
void Indicator::init(void) {
  printf("[v] Indicator handler inited\n");
  DriverDisplayC::instance()->print("[v] " + getName() + " initialized.\n");
  vTaskDelay(1000 / portTICK_PERIOD_MS); // TODO: why sleep here?
}
// -------------
// FreeRTOS TASK
// -------------
void Indicator::task() {
  // do not add code here -- only controlling the blink frequency
  // polling loop
  DriverDisplayC *dd = DriverDisplayC::instance();
  while (1) {
    dd->indicator_set_and_blink(carState.Indicator.get(), blinkState);
    blinkState = !blinkState;

    // sleep
    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}
