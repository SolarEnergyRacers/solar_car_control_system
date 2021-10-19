//
// Car State with all car information
//

#include <CarState.h>
#include <IOExt.h>
#include <Indicator.h>
#include <definitions.h>

using namespace std;

extern CarState carState;

int CarState::getIdx(string pinName) { return idxOfPin.find(pinName)->second; }
CarStatePin *CarState::getPin(int devNr, int pinNr) { return &(carState.pins[IOExt::getIdx(devNr, pinNr)]); }
CarStatePin *CarState::getPin(int port) { return &(carState.pins[IOExt::getIdx(port)]); }
CarStatePin *CarState::getPin(string pinName) { return &(carState.pins[carState.getIdx(pinName)]); }

static const char *INDICATOR_str[] = {"off", "left", "right", "HAZARD FLASHR"};
static const char *CONSTANT_MODE_str[] = {"none", "speed", "power"};
static const char *DRIVE_DIRECTION_str[] = {"fwd", "bwd"};
static const char *BOOL_str[] = {"false", "true"};
static const char *LIGHT_str[] = {"off", "L1", "L2"};

const string CarState::print(string msg) {
  stringstream ss(msg);
  if (msg.length() > 0)
    ss << msg << endl;
  // ss << ss.fixed << ss.precision(3) << ss.width(7);
  ss << "Speed ........... " << Speed.get() << endl;
  ss << "Acceleration .... " << Acceleration.get() << endl;
  ss << "Deceleration .... " << Deceleration.get() << endl;
  ss << "Battery ......... " << Battery.get() << endl;
  ss << "Photo Voltaic ... " << PhotoVoltaic.get() << endl;
  ss << "Motor ........... " << Motor.get() << endl;
  ss << "Drive Direction . " << DRIVE_DIRECTION_str[(int)(DriveDirection.get())] << endl;
  ss << "------------------------" << endl;
  ss << "Indicator ....... " << INDICATOR_str[(int)(Indicator.get())] << endl;
  ss << "Constant Mode On  " << BOOL_str[(int)(ConstantModeOn.get())] << endl;
  ss << "Constant Mode ... " << CONSTANT_MODE_str[(int)(ConstantMode.get())] << endl;
  ss << "Target Speed .... " << TargetSpeed.get() << endl;
  ss << "Target Power .... " << TargetPower.get() << endl;
  ss << "Info Last ....... " << InfoLast.get() << endl;
  ss << "Light .... ...... " << LIGHT_str[(int)(Light.get())] << endl;
  ss << "=======================" << endl;
  ss << printIOs("");
  return ss.str();
}

const string CarState::serialize(string msg) {
  cJSON *carData = cJSON_CreateObject();
  cJSON *dynData = cJSON_CreateObject();
  cJSON *ctrData = cJSON_CreateObject();
  cJSON_AddItemToObject(carData, "dynamicData", dynData);
  cJSON_AddNumberToObject(dynData, "speed", Speed.get());
  cJSON_AddNumberToObject(dynData, "battery", Battery.get());
  cJSON_AddStringToObject(dynData, "indicator", INDICATOR_str[(int)(Indicator.get())]);

  cJSON_AddItemToObject(carData, "controlData", ctrData);
  cJSON_AddNumberToObject(ctrData, "targetSpeed", TargetSpeed.get());
  cJSON_AddStringToObject(ctrData, "infoLast", InfoLast.get().c_str());
  cJSON_AddStringToObject(ctrData, "light", LIGHT_str[(int)(Light.get())]);
  cJSON_AddStringToObject(ctrData, "ios:", printIOs("").c_str());
  return cJSON_Print(carData);
}

const string CarState::printIOs(string msg) {
  stringstream ss(msg);
  if (msg.length() > 0)
    ss << msg << endl;
  for (int devNr = 0; devNr < PCF8574_NUM_DEVICES; devNr++) {
    // printf("0x%2x0: ", devNr);
    ss << devNr << ": ";
    for (int pinNr = 0; pinNr < PCF8574_NUM_PORTS; pinNr++) {
      string color = "";
      CarStatePin *pin = carState.getPin(devNr, pinNr);
      if (pin->mode == OUTPUT) {
        ss << " \033[1;31m" << pin->value << "\033[0;39m";
      } else {
        ss << pin->value;
      }
      if ((IOExt::getIdx(devNr, pinNr) + 1) % 8 == 0)
        ss << " | ";
      else if ((IOExt::getIdx(devNr, pinNr) + 1) % 4 == 0)
        // printf(" - ");
        ss << " - ";
    }
  }
  ss << endl;
  return ss.str();
}
