//
// Car State with all car information
//

#include <CarState.h>
#include <definitions.h>

using namespace std;

const string CarState::print(string msg) {
  stringstream ss(msg);
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
  ss << "Constant Mode ... " << CONSTANT_MODE_str[(int)(ConstantMode.get())] << endl;
  ss << "Target Speed .... " << TargetSpeed.get() << endl;
  ss << "Target Power .... " << TargetPower.get() << endl;
  ss << "Info Last ....... " << InfoLast.get() << endl;
  ss << "Light 1 On ...... " << BOOL_str[Light1On.get()] << endl;
  ss << "Light 2 On ...... " << BOOL_str[Light2On.get()] << endl;
  //ss << "Indicator Left .. " << ON_OFF_str[IndicatorLeft.get()] << endl;
  //ss << "Indicator Right . " << ON_OFF_str[IndicatorRight.get()] << endl;
  ss << "=======================" << endl;
  return ss.str();
}

const string CarState::serialize(string msg) {
  cJSON *carData = cJSON_CreateObject();
  cJSON *dynData = cJSON_CreateObject();
  cJSON *ctrData = cJSON_CreateObject();
  cJSON_AddItemToObject(carData, "dynamicData", dynData);
  cJSON_AddNumberToObject(dynData, "speed", Speed.get());
  cJSON_AddNumberToObject(dynData, "battery", Battery.get());

  cJSON_AddItemToObject(carData, "controlData", ctrData);
  cJSON_AddNumberToObject(ctrData, "TargetSpeed", TargetSpeed.get());
  cJSON_AddStringToObject(ctrData, "InfoLast", InfoLast.get().c_str());
  return cJSON_Print(carData);
}