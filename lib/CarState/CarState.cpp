//
// Car State with all car information
//
#include <fmt/core.h>
#include <fmt/format.h>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>

#include <CarState.h>
#include <Helper.h>
#include <IOExt2.h>
#include <Indicator.h>
#include <definitions.h>

using namespace std;

extern CarState carState;

int CarState::getIdx(string pinName) { return idxOfPin.find(pinName)->second; }
CarStatePin *CarState::getPin(int devNr, int pinNr) { return &(carState.pins[IOExt2::getIdx(devNr, pinNr)]); }
CarStatePin *CarState::getPin(int port) { return &(carState.pins[IOExt2::getIdx(port)]); }
CarStatePin *CarState::getPin(string pinName) { return &(carState.pins[carState.getIdx(pinName)]); }

static const char *INDICATOR_str[] = {"OFF", "LEFT", "RIGHT", "HAZARD FLASHR"};
static const char *CONSTANT_MODE_str[] = {"NONE", "SPEED", "POWER"};
static const char *DRIVE_DIRECTION_str[] = {"fwd", "bwd"};
static const char *BOOL_str[] = {"false", "true"};
static const char *LIGHT_str[] = {"OFF", "L1", "L2"};
static const char *INFO_TYPE_str[] = {"INFO", "STATUS", "WARN", "ERROR"};
static const char *SPEED_ARROW_str[]{"OFF", "INCREASE", "DECREASE"};

void CarState::init_values() {
  Speed = 0;
  Acceleration = 0;
  Deceleration = 0;
  BatteryVoltage = 0;
  BatteryCurrent = 0;
  PhotoVoltaicCurrent = 0;
  MotorCurrent = 0;

  Indicator = INDICATOR::OFF;
  DriveDirection = DRIVE_DIRECTION::FORWARD;
  ConstantMode = CONSTANT_MODE::SPEED;
  ConstantModeOn = false;
  SdCardDetect = false;

  TargetSpeed = 0;
  TargetPower = 0;
  DriverInfo = "ok.";
  DriverInfoType = INFO_TYPE::STATUS;
  Light = LIGHT::OFF;
}

const string CarState::print(string msg, bool withColors) {
  time_t theTime = time(NULL);
  struct tm t = *localtime(&theTime);

  stringstream ss(msg);
  ss << "====SER4 Car Status====" << VERSION << "==" << VERSION_PUBLISHED << "====";
  ss << t.tm_year << "." << t.tm_mon << "." << t.tm_mday << "_" << t.tm_hour << ":" << t.tm_min << ":" << t.tm_sec;
  ss << "====uptime: " << getTimeStamp(millis() / 1000) << "s====" << asctime(&t);
  if (msg.length() > 0)
    ss << msg << endl;
  // ss << ss.fixed << ss.precision(3) << ss.width(7)
  ss << "Display Status ........ " << DISPLAY_STATUS_str[(int)displayStatus] << endl;
  ss << "Speed ................. " << Speed << endl;
  ss << "Acceleration locked ... " << BOOL_str[(int)(AccelerationLocked)] << endl;
  ss << "Acceleration .......... " << Acceleration << endl;
  ss << "Deceleration .......... " << Deceleration << endl;
  ss << "Acceleration Display .. " << AccelerationDisplay << endl;
  ss << "Break pedal pressed ... " << BOOL_str[(int)(BreakPedal)] << endl;
  ss << "Battery On ............ " << BatteryOn << endl;
  ss << "Battery Voltage ....... " << BatteryVoltage << endl;
  ss << "Battery Current ....... " << BatteryCurrent << endl;
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
  ss << "SD Card detected....... " << BOOL_str[(int)(SdCardDetect)] << endl;
  ss << "Info Last ............. "
     << "[" << INFO_TYPE_str[(int)DriverInfoType] << "] " << DriverInfo << endl;
  ss << "Speed Arrow ........... " << SPEED_ARROW_str[(int)SpeedArrow] << "]" << endl;
  ss << "Light ................. " << LIGHT_str[(int)(Light)] << endl;
  ss << "IO ....................." << printIOs("", false) << endl;
  ss << "===========================================================================================" << endl;
  return ss.str();
}

const string CarState::serialize(string msg) {
  time_t theTime = time(NULL);
  struct tm t = *localtime(&theTime);
  string timeStamp = asctime(&t);
  timeStamp.erase(timeStamp.end() - 1);

  cJSON *carData = cJSON_CreateObject();
  cJSON *dynData = cJSON_CreateObject();
  cJSON *ctrData = cJSON_CreateObject();

  cJSON_AddItemToObject(carData, "dynamicData", dynData);
  cJSON_AddStringToObject(dynData, "timeStamp", timeStamp.c_str());
  cJSON_AddStringToObject(dynData, "uptime", getTimeStamp(millis() / 1000).c_str());
  cJSON_AddStringToObject(dynData, "msg", msg.c_str());
  cJSON_AddNumberToObject(dynData, "speed", Speed);
  cJSON_AddNumberToObject(dynData, "acceleration", Acceleration);
  cJSON_AddNumberToObject(dynData, "deceleration", Deceleration);
  cJSON_AddNumberToObject(dynData, "accelerationDisplay", AccelerationDisplay);
  cJSON_AddBoolToObject(dynData, "batteryOn", BatteryOn);
  cJSON_AddNumberToObject(dynData, "batteryVoltage", floor(BatteryVoltage * 1000.0 + .5) / 1000.0);
  cJSON_AddNumberToObject(dynData, "batteryCurrent", floor(BatteryCurrent * 1000.0 + .5) / 1000.0);
  cJSON_AddBoolToObject(dynData, "pvOn", PhotoVoltaicOn);
  cJSON_AddNumberToObject(dynData, "pvCurrent", floor(PhotoVoltaicCurrent * 1000.0 + .5) / 1000.0);
  cJSON_AddBoolToObject(dynData, "motorOn", MotorOn);
  cJSON_AddNumberToObject(dynData, "motorCurrent", floor(MotorCurrent * 1000.0 + .5) / 1000.0);

  cJSON_AddNumberToObject(dynData, "mppt1Current", floor(Mppt1Current * 1000.0 + .5) / 1000.0);
  cJSON_AddNumberToObject(dynData, "mppt2Current", floor(Mppt2Current * 1000.0 + .5) / 1000.0);
  cJSON_AddNumberToObject(dynData, "mppt3Current", floor(Mppt3Current * 1000.0 + .5) / 1000.0);
  cJSON_AddNumberToObject(dynData, "voltageMin", floor(Umin * 1000.0 + .5) / 1000.0);
  cJSON_AddNumberToObject(dynData, "voltageAvg", floor(Uavg * 1000.0 + .5) / 1000.0);
  cJSON_AddNumberToObject(dynData, "voltageMax", floor(Umax * 1000.0 + .5) / 1000.0);
  cJSON_AddNumberToObject(dynData, "t1", floor(T1 * 1000.0 + .5) / 1000.0);
  cJSON_AddNumberToObject(dynData, "t2", floor(T2 * 1000.0 + .5) / 1000.0);
  cJSON_AddNumberToObject(dynData, "t3", floor(T3 * 1000.0 + .5) / 1000.0);
  cJSON_AddNumberToObject(dynData, "t4", floor(T4 * 1000.0 + .5) / 1000.0);

  cJSON_AddStringToObject(dynData, "indicator", INDICATOR_str[(int)(Indicator)]);
  cJSON_AddStringToObject(dynData, "driveDirection", DRIVE_DIRECTION_str[(int)(DriveDirection)]);
  cJSON_AddStringToObject(dynData, "constantModeOn", BOOL_str[(int)(ConstantModeOn)]);

  cJSON_AddStringToObject(dynData, "sdCardDetect", BOOL_str[(int)(SdCardDetect)]);

  cJSON_AddItemToObject(carData, "controlData", ctrData);
  cJSON_AddStringToObject(ctrData, "displayStatus", DISPLAY_STATUS_str[(int)displayStatus]);
  cJSON_AddStringToObject(ctrData, "constantMode", CONSTANT_MODE_str[(int)(ConstantMode)]);
  cJSON_AddNumberToObject(ctrData, "targetSpeed", TargetSpeed);
  cJSON_AddNumberToObject(ctrData, "targetPower", TargetPower);
  cJSON_AddStringToObject(ctrData, "driverInfo", fmt::format("[{}] {}", INFO_TYPE_str[(int)DriverInfoType], DriverInfo.c_str()).c_str());
  cJSON_AddStringToObject(ctrData, "speedArrow", SPEED_ARROW_str[(int)SpeedArrow]);
  cJSON_AddStringToObject(ctrData, "light", LIGHT_str[(int)(Light)]);
  cJSON_AddStringToObject(ctrData, "io:", printIOs("", false).c_str());
  return fmt::format("{}\n", cJSON_PrintUnformatted(carData));
}

const string CarState::csv(string msg, bool withHeader) {
  time_t theTime = time(NULL);
  struct tm t = *localtime(&theTime);
  string timeStamp = asctime(&t);
  timeStamp.erase(timeStamp.end() - 1);

  stringstream ss(msg);
  if (withHeader) {
    // header
    ss << "timeStamp, ";
    ss << "uptime, ";
    ss << "msg, ";
    ss << "speed, ";
    ss << "acceleration, ";
    ss << "deceleration, ";
    ss << "accelerationDisplay, ";
    ss << "batteryOn, ";
    ss << "batteryVoltage, ";
    ss << "batteryCurrent, ";
    ss << "pvOn, ";
    ss << "pvCurrent, ";
    ss << "motorOn, ";
    ss << "motorCurrent, ";

    ss << "mppt1Current, ";
    ss << "mppt2Current, ";
    ss << "mppt3Current, ";
    ss << "voltageMin, ";
    ss << "voltageAvg, ";
    ss << "voltageMax, ";
    ss << "t1, ";
    ss << "t2, ";
    ss << "t3, ";
    ss << "t4, ";

    ss << "indicator, ";
    ss << "driveDirection, ";
    ss << "constantModeOn, ";
    ss << "sdCardDetected, ";

    ss << "displayStatus, ";
    ss << "constantMode, ";
    ss << "targetSpeed, ";
    ss << "targetPower, ";
    ss << "driverInfo, ";
    ss << "speedArrow, ";
    ss << "light, ";
    ss << "io ";
    ss << endl;
  }
  // data
  ss << timeStamp.c_str() << ", ";
  ss << getTimeStamp(millis() / 1000).c_str() << ", ";
  ss << msg.c_str() << ", ";
  ss << Speed << ", ";
  ss << Acceleration << ", ";
  ss << Deceleration << ", ";
  ss << AccelerationDisplay << ", ";
  ss << BatteryOn << ", ";
  ss << floor(BatteryVoltage * 1000.0 + .5) / 1000.0 << ", ";
  ss << floor(BatteryCurrent * 1000.0 + .5) / 1000.0 << ", ";
  ss << PhotoVoltaicOn << ", ";
  ss << floor(PhotoVoltaicCurrent * 1000.0 + .5) / 1000.0 << ", ";
  ss << MotorOn << ", ";
  ss << floor(MotorCurrent * 1000.0 + .5) / 1000.0 << ", ";

  ss << floor(Mppt1Current * 1000.0 + .5) / 1000.0 << ", ";
  ss << floor(Mppt2Current * 1000.0 + .5) / 1000.0 << ", ";
  ss << floor(Mppt3Current * 1000.0 + .5) / 1000.0 << ", ";
  ss << floor(Umin * 1000.0 + .5) / 1000.0 << ", ";
  ss << floor(Uavg * 1000.0 + .5) / 1000.0 << ", ";
  ss << floor(Umax * 1000.0 + .5) / 1000.0 << ", ";
  ss << floor(T1 * 1000.0 + .5) / 1000.0 << ", ";
  ss << floor(T2 * 1000.0 + .5) / 1000.0 << ", ";
  ss << floor(T3 * 1000.0 + .5) / 1000.0 << ", ";
  ss << floor(T4 * 1000.0 + .5) / 1000.0 << ", ";

  ss << INDICATOR_str[(int)(Indicator)] << ", ";
  ss << DRIVE_DIRECTION_str[(int)(DriveDirection)] << ", ";
  ss << BOOL_str[(int)(ConstantModeOn)] << ", ";
  ss << BOOL_str[(int)(SdCardDetect)] << ", ";

  ss << DISPLAY_STATUS_str[(int)displayStatus] << ", ";
  ss << CONSTANT_MODE_str[(int)(ConstantMode)] << ", ";
  ss << TargetSpeed << ", ";
  ss << TargetPower << ", ";
  ss << fmt::format("[{}] {}", INFO_TYPE_str[(int)DriverInfoType], DriverInfo.c_str()).c_str() << ", ";
  ss << SPEED_ARROW_str[(int)SpeedArrow] << ", ";
  ss << LIGHT_str[(int)(Light)] << ", ";
  ss << printIOs("", false).c_str() << ", ";
  ss << endl;
  return ss.str();
}

const string CarState::printIOs(string msg, bool withColors, bool deltaOnly) {
  string normalColor = "\033[0;39m";
  string highLightColorOut = "\033[1;31m";
  string highLightColorChg = "\033[1;36m";
  stringstream ss(msg);
  if (msg.length() > 0)
    ss << msg << endl;

  bool hasDelta = false;
  for (int devNr = 0; devNr < MCP23017_NUM_DEVICES; devNr++) {
    ss << devNr << ": ";
    for (int pinNr = 0; pinNr < MCP23017_NUM_PORTS; pinNr++) {
      int idx = IOExt2::getIdx(devNr, pinNr);
      CarStatePin *pin = carState.getPin(devNr, pinNr);
      if (pin->mode == OUTPUT && withColors) {
        if (pin->value != pin->oldValue) {
          hasDelta = true;
          ss << highLightColorChg << pin->value << normalColor;
        } else {
          ss << highLightColorOut << pin->value << normalColor;
        }
      } else {
        if (pin->value != pin->oldValue) {
          hasDelta = true;
          ss << highLightColorChg << pin->value << normalColor;
        } else {
          ss << pin->value;
        }
      }
      if (idx < MCP23017_NUM_DEVICES * MCP23017_NUM_PORTS - 1) {
        if ((idx + 1) % 8 == 0)
          ss << " | ";
        else if ((idx + 1) % 4 == 0)
          ss << "-";
      }
    }
  }

  // ss << endl;
  if (hasDelta || !deltaOnly)
    return ss.str();
  else
    return "";
}
