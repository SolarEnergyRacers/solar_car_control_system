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
#include <ConfigFile.h>
#include <Console.h>
#include <Helper.h>
#include <IOExt.h>
#include <Indicator.h>
#include <SDCard.h>
#include <definitions.h>

using namespace std;

extern CarState carState;
extern Console console;
extern SDCard sdCard;
extern IOExt ioExt;

int CarState::getIdx(string pinName) { return idxOfPin.find(pinName)->second; }
CarStatePin *CarState::getPin(int devNr, int pinNr) { return &(carState.pins[IOExt::getIdx(devNr, pinNr)]); }
CarStatePin *CarState::getPin(int port) { return &(carState.pins[IOExt::getIdx(port)]); }
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
  ReferenceSolarCell = 0;
  MotorCurrent = 0;

  Indicator = INDICATOR::OFF;
  DriveDirection = DRIVE_DIRECTION::FORWARD;
  ConstantMode = CONSTANT_MODE::SPEED;
  ConstantModeOn = false; //#SAFETY#: deceleration unlock const mode
  SdCardDetect = false;

  TargetSpeed = 0;
  TargetPower = 0;
  DriverInfo = "Acceleration\nstill locked!";
  DriverInfoType = INFO_TYPE::STATUS;
  Light = LIGHT::OFF;

  console << "Reread all IOs in foreced mode...\n";
  ioExt.readAll(false, true);
  // console << print("Initial State") << "\n";

  // read from ser4config.ini file
  initalize_config();
  // console << print("State after SER4CONF.INI") << "\n";
}

bool CarState::initalize_config() {
  try {
    ConfigFile cf = ConfigFile(FILENAME_SER4CONFIG);
    // [Main]
    LogFilename = cf.get("Main", "LogFilename", "/ser4data.csv");
    LogFilePeriod = cf.get("Main", "LogFilePeriod", 1);
    LogInterval = cf.get("Main", "LogInterval", 1);
    // [PID]
    Kp = cf.get("PID", "Kp", 2);
    Ki = cf.get("PID", "Ki", 1);
    Kd = cf.get("PID", "Kd", 0.1);
    // [Dynamic]
    PaddleDamping = cf.get("Dynamic", "PaddleDamping", 10);
    PaddleOffset = cf.get("Dynamic", "PaddleOffset", 3000);
    PaddleAdjustCounter = cf.get("Dynamic", "PaddleAdjustCounter", 25);
    ConstSpeedIncrease = cf.get("Dynamic", "ConstSpeedIncrease", 1.0);
    ConstPowerIncrease = cf.get("Dynamic", "ConstPowerIncrease", 0.5);
    // [Communication]
    // I2CFrequence = cf.get("Communication", "I2CFrequence", 50);
    CarDataLogPeriod = cf.get("Communication", "CarDataLogPeriod", 1000);
    Serial1Baudrate = cf.get("Communication", "Serail1Baudrate", 115200);
    Serial2Baudrate = cf.get("Communication", "Serial2Baudrate", 115200);
    // [Telemetry]
    SendInterval = cf.get("Telemetry", "", 1000);
    MaxCachedRecords = cf.get("Telemetry", "MaxCachedRecords", 100);

  } catch (exception &ex) {
    console << "WARN: No configfile: '" << FILENAME_SER4CONFIG << "' found: " << ex.what() << "\n";
    return false;
  }
  return true;
}

const string CarState::print(string msg, bool withColors) {
  time_t theTime = time(NULL);
  struct tm t = *localtime(&theTime);

  stringstream ss(msg);
  ss << "====SER4 Car Status====" << VERSION << "==";
  ss << t.tm_year << "." << t.tm_mon << "." << t.tm_mday << "_" << t.tm_hour << ":" << t.tm_min << ":" << t.tm_sec;
  ss << "====uptime:" << getTimeStamp(millis() / 1000) << "s====" << asctime(&t) << "==";
  if (msg.length() > 0)
    ss << msg << endl;
  ss << "Display Status ........ " << DISPLAY_STATUS_str[(int)displayStatus] << endl;
  ss << "Speed ................. " << Speed << endl;
  ss << "Acceleration locked ... " << BOOL_str[(int)(AccelerationLocked)] << endl;
  ss << "Acceleration .......... " << Acceleration << endl;
  ss << "Deceleration .......... " << Deceleration << endl;
  ss << "Acceleration Display... " << AccelerationDisplay << endl;
  ss << "Break pedal pressed ... " << BOOL_str[(int)(BreakPedal)] << endl;
  ss << "Battery On............. " << BatteryOn << endl;
  ss << "Battery Voltage........ " << BatteryVoltage << endl;
  ss << "Battery Current........ " << BatteryCurrent << endl;
  ss << "Battery Errors ........." << batteryErrorsAsString(true) << endl;
  ss << "Battery Precharge State " << PRECHARGE_STATE_str[(int)(PrechargeState)] << endl;
  ss << "Photo Voltaic On ...... " << PhotoVoltaicOn << endl;
  ss << "MPPT1 Current ......... " << Mppt1Current << endl;
  ss << "MPPT2 Current ......... " << Mppt2Current << endl;
  ss << "MPPT3 Current ......... " << Mppt3Current << endl;
  ss << "Photo Voltaic Current . " << PhotoVoltaicCurrent << endl;
  ss << "Photo Reference Cell .. " << ReferenceSolarCell << endl;
  ss << "Acceleration Display .. " << AccelerationDisplay << endl;
  ss << "Break pedal pressed ... " << BOOL_str[(int)(BreakPedal)] << endl;
  ss << "Battery On ............ " << BatteryOn << endl;
  ss << "Battery Voltage ....... " << BatteryVoltage << endl;
  ss << "Battery Current ....... " << BatteryCurrent << endl;
  ss << "Photo Voltaic On ...... " << PhotoVoltaicOn << endl;
  ss << "Motor On .............. " << MotorOn << endl;
  ss << "Motor Current ......... " << MotorCurrent << endl;
  ss << "Drive Direction ....... " << DRIVE_DIRECTION_str[(int)(DriveDirection)] << endl;
  ss << "------------------------" << endl;
  ss << "Indicator ............. " << INDICATOR_str[(int)(Indicator)] << endl;
  ss << "Constant Mode On ...... " << BOOL_str[(int)(ConstantModeOn)] << endl;
  ss << "Constant Mode ......... " << CONSTANT_MODE_str[(int)(ConstantMode)] << endl;
  ss << "Target Speed .......... " << TargetSpeed << endl;
  ss << "Target Power .......... " << TargetPower << endl;
  ss << "SD Card detected....... " << BOOL_str[(int)(SdCardDetect)] << "(" << SdCardDetect << ")" << endl;
  ss << "Info Last ............. "
     << "[" << INFO_TYPE_str[(int)DriverInfoType] << "] " << DriverInfo << endl;
  ss << "Speed Arrow ........... " << SPEED_ARROW_str[(int)SpeedArrow] << endl;
  ss << "Light ................. " << LIGHT_str[(int)(Light)] << endl;
  ss << "IO .................... " << printIOs("", false) << endl;

  ss << "Log file name ......... " << LogFilename << endl;
  ss << "Log file persiod ...... " << LogFilePeriod << endl;
  ss << "Log file interval ..... " << LogInterval << endl;

  // [TaskTimings]
  ss << "Sleep time EIOExt ..... " << SleepTimeIOExt << endl;

  // [PID]
  ss << "Kp .................... " << Kp << endl;
  ss << "Ki .................... " << Ki << endl;
  ss << "Kd .................... " << Kd << endl;

  // [Dynamic]
  ss << "Paddle damping ........ " << PaddleDamping << endl;
  ss << "Paddle offset ......... " << PaddleOffset << endl;
  ss << "Paddle adjustment ..... " << PaddleAdjustCounter << endl;
  ss << "Const speed increase .. " << ConstSpeedIncrease << endl;
  ss << "Const power invrease .. " << ConstPowerIncrease << endl;

  // [Communication]
  ss << "I2C frequency ......... " << I2CFrequence << endl;
  ss << "Car data log period ... " << CarDataLogPeriod << endl;
  ss << "Serial 1 baud rate .... " << Serial1Baudrate << endl;
  ss << "Serial 2 baud rate .... " << Serial2Baudrate << endl;

  // [Telemetry]
  ss << "Telemetry send intervall" << SendInterval << endl;
  ss << "Telemetry cache records " << MaxCachedRecords << endl;

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
    ss << "batteryErrors, ";
    ss << "batteryPrechargeState, ";
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
    ss << "T1, ";
    ss << "T2, ";
    ss << "T3, ";
    ss << "T4, ";

    ss << "indicator, ";
    ss << "driveDirection, ";
    ss << "constantModeOn, ";
    ss << "sdCardDetected, ";

    ss << "displayStatus, ";

    ss << "constantMode, ";
    ss << "targetSpeed, ";
    ss << "Kp, ";
    ss << "Ki, ";
    ss << "Kd, ";
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
  ss << batteryErrorsAsString() << ", ";
  ss << PRECHARGE_STATE_str[(int)(PrechargeState)] << ", ";
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
  ss << Kp << ", ";
  ss << Ki << ", ";
  ss << Kd << ", ";
  ss << TargetPower << ", ";
  string field = DriverInfo.replace(DriverInfo.begin(), DriverInfo.end(), '\n', ' ');
  ss << fmt::format("[{}] {}", INFO_TYPE_str[(int)DriverInfoType], field) << ", ";
  ss << SPEED_ARROW_str[(int)SpeedArrow] << ", ";
  ss << LIGHT_str[(int)(Light)] << ", ";
  ss << printIOs("", false).c_str() << ", ";
  ss << endl;
  return ss.str();
}

const string CarState::printIOs(string msg, bool withColors, bool deltaOnly) {
  string normalColor = "\033[0;39m";
  string highLightColorOut = "\033[1;31m"; // red
  string highLightColorChg = "\033[1;36m"; // blue
  stringstream ss(msg);
  if (msg.length() > 0)
    ss << msg << endl;

  bool hasDelta = false;
  for (int devNr = 0; devNr < MCP23017_NUM_DEVICES; devNr++) {
    ss << devNr << ": ";
    for (int pinNr = 0; pinNr < MCP23017_NUM_PORTS; pinNr++) {
      int idx = IOExt::getIdx(devNr, pinNr);
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

const string CarState::batteryErrorsAsString(bool verbose /*= false*/) {
  stringstream ss;

  ss << "[";

  if (verbose) {
    for (auto const &battErr : BatteryErrors) {
      ss << BATTERY_ERROR_str[(int)(battErr)] << "-";
    }
  } else {
    for (auto const &battErr : BatteryErrors) {
      ss << (int)(battErr) << "-";
    }
  }

  ss << "]";

  return ss.str();
}