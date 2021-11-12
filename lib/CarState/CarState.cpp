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
static const char *INFO_TYPE_str[] = {"INFO", "STATUS", "WARN", "ERROR"};
static const char *SPEED_ARROW_str[]{"OFF", "INCREASE", "DECREASE"};

const string CarState::print(string msg, bool withColors) {
  time_t theTime = time(NULL);
  struct tm t = *localtime(&theTime);

  stringstream ss(msg);
  ss << "====SER4 Car Status====" << VERSION << "==" << VERSION_PUBLISHED << "====";
  ss << t.tm_year << "." << t.tm_mon << "." << t.tm_mday << "_" << t.tm_hour << ":" << t.tm_min << ":" << t.tm_sec;
  ss << "====" << millis() / 1000 << "s====" << asctime(&t) << endl;
  if (msg.length() > 0)
    ss << msg << endl;
  // ss << ss.fixed << ss.precision(3) << ss.width(7);
  ss << "Speed ................. " << Speed << endl;
  ss << "Acceleration .......... " << Acceleration << endl;
  ss << "Deceleration .......... " << Deceleration << endl;
  ss << "Acceleration Display... " << AccelerationDisplay << endl;
  ss << "Battery On............. " << BatteryOn << endl;
  ss << "Battery Voltage........ " << BatteryVoltage << endl;
  ss << "Battery Current........ " << BatteryCurrent << endl;
  ss << "Photo Voltaic On ...... " << PhotoVoltaicOn << endl;
  ss << "Photo Voltaic Current . " << PhotoVoltaicCurrent << endl;
  ss << "Motor On .............. " << MotorOn << endl;
  ss << "Motor Current ......... " << MotorCurrent << endl;
  ss << "Drive Direction ....... " << DRIVE_DIRECTION_str[(int)(DriveDirection)] << endl;
  ss << "------------------------" << endl;
  ss << "Indicator ............. " << INDICATOR_str[(int)(Indicator)] << endl;
  ss << "Constant Mode On ...... " << BOOL_str[(int)(ConstantModeOn)] << endl;
  ss << "Constant Mode ......... " << CONSTANT_MODE_str[(int)(ConstantMode)] << endl;
  ss << "Target Speed .......... " << TargetSpeed << endl;
  ss << "Target Power .......... " << TargetPower << endl;
  ss << "Info Last ............. "
     << "[" << INFO_TYPE_str[(int)DriverInfoType] << "] " << DriverInfo << endl;
  ss << "Spped Arrow ........... " << SPEED_ARROW_str[(int)SpeedArrow] << "]" << endl;
  ss << "Light ................. " << LIGHT_str[(int)(Light)] << endl;
  ss << "IO ....................." << printIOs("", false);
  ss << "===========================================================================================" << endl;
  return ss.str();
}

const string CarState::serialize(string msg) {
  cJSON *carData = cJSON_CreateObject();
  cJSON *dynData = cJSON_CreateObject();
  cJSON *ctrData = cJSON_CreateObject();
  cJSON_AddItemToObject(carData, "dynamicData", dynData);
  cJSON_AddNumberToObject(dynData, "speed", Speed);
  cJSON_AddBoolToObject(dynData, "batteryOn", BatteryOn);
  cJSON_AddNumberToObject(dynData, "batteryVoltage", BatteryVoltage);
  cJSON_AddNumberToObject(dynData, "batteryCurrent", BatteryCurrent);
  cJSON_AddBoolToObject(dynData, "pvOn", PhotoVoltaicOn);
  cJSON_AddNumberToObject(dynData, "pvCurrent", PhotoVoltaicCurrent);
  cJSON_AddBoolToObject(dynData, "motorOn", MotorOn);
  cJSON_AddNumberToObject(dynData, "motorCurrent", MotorCurrent);

  cJSON_AddStringToObject(dynData, "indicator", INDICATOR_str[(int)(Indicator)]);

  cJSON_AddItemToObject(carData, "controlData", ctrData);
  cJSON_AddNumberToObject(ctrData, "targetSpeed", TargetSpeed);

  char buf[100];
  snprintf(buf, 100, "[%s] %s", INFO_TYPE_str[(int)DriverInfoType], DriverInfo.c_str());
  cJSON_AddStringToObject(ctrData, "driverInfo", buf);

  cJSON_AddStringToObject(ctrData, "light", LIGHT_str[(int)(Light)]);
  cJSON_AddStringToObject(ctrData, "io:", printIOs("", false).c_str());
  return cJSON_Print(carData);
}

const string CarState::printIOs(string msg, bool withColors) {
  string normalColor = "\033[0;39m";
  string highLightColor = "\033[1;31m";
  stringstream ss(msg);
  if (msg.length() > 0)
    ss << msg << endl;
  for (int devNr = 0; devNr < PCF8574_NUM_DEVICES; devNr++) {
    // printf("0x%2x0: ", devNr);
    ss << devNr << ": ";
    for (int pinNr = 0; pinNr < PCF8574_NUM_PORTS; pinNr++) {
      CarStatePin *pin = carState.getPin(devNr, pinNr);
      if (pin->mode == OUTPUT && withColors) {
        ss << " " << highLightColor << pin->value << normalColor;
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
