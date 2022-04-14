//
// Car State with all car information
//

#ifndef CARSTATE_H
#define CARSTATE_H

#include <cJSON.h>
#include <list>
#include <map>
#include <sstream>
#include <string>

#include <definitions.h>

#include <CarStatePin.h>

using namespace std;

// public structures
enum class INDICATOR { OFF, LEFT, RIGHT, WARN };
enum class INFO_TYPE { INFO, STATUS, WARN, ERROR };
enum class SPEED_ARROW { OFF, INCREASE, DECREASE };
enum class CONSTANT_MODE { NONE, SPEED, POWER };
enum class DRIVE_DIRECTION { FORWARD, BACKWARD };
enum class LIGHT { OFF, L1, L2 };
enum class DISPLAY_STATUS {
  DRIVER_HALTED,
  DRIVER_SETUP,
  DRIVER_BACKGROUND,
  DRIVER_RUNNING,
  DRIVER_DEMOSCREEN,
  ENGINEER_CONSOLE,
  ENGINEER_HALTED,
  ENGINEER_SETUP,
  ENGINEER_BACKGROUND,
  ENGINEER_RUNNING
};

static const char *DISPLAY_STATUS_str[] = {
    "DRIVER_HALTED",       // no action on this display
    "DRIVER_SETUP",        // driver screen setup
    "DRIVER_BACKGROUND",   // create background for driver screen
    "DRIVER_RUNNING",      // driver mode active
    "DRIVER_DEMOSCREEN",   // demo
    "ENGINEER_CONSOLE",    // console mode (engineer screen)
    "ENGINEER_HALTED"      // no action on this display
    "ENGINEER_SETUP",      // engineerscreen setup
    "ENGINEER_BACKGROUND", // create background for engineer screen
    "ENGINEER_RUNNING"     // enineer mode active
};

enum class PRECHARGE_STATE { ERROR, IDLE, MEASURE, PRECHARGE, RUN, ENABLE_PACK };

static const char *PRECHARGE_STATE_str[] = {"ERROR", "IDLE", "MEASURE", "PRECHARGE", "RUN", "ENABLE_PACK"};

enum class BATTERY_ERROR {
  CELL_OVER_VOLTAGE,
  CELL_UNDER_VOLTAGE,
  CELL_OVER_TEMP,
  MEASUREMENT_UNTRUSTED,
  CMU_COMM_TIMEOUT,
  VEHICLE_COMM_TIMEOUT,
  BMU_IN_SETUP_MODE,
  CMU_CAN_POWER,
  PACK_ISOLATION_TEST_FAIL,
  SOC_MEASUREMENT_INVALID,
  CAN_12V_LOW,
  CONTACTOR_STUCK,
  EXTRA_CELL_DETECTED
};

static const char *BATTERY_ERROR_str[] = {
    "CELL_OVER_VOLTAGE",    "CELL_UNDER_VOLTAGE", "CELL_OVER_TEMP",     "MEASUREMENT_UNTRUSTED",    "CMU_COMM_TIMEOUT",
    "VEHICLE_COMM_TIMEOUT", "BMU_IN_SETUP_MODE",  "CMU_CAN_POWER",      "PACK_ISOLATION_TEST_FAIL", "SOC_MEASUREMENT_INVALID",
    "CAN_12V_LOW",          "CONTACTOR_STUCK",    "EXTRA_CELL_DETECTED"};

class CarState {

public:
  CarState() {
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

    ConstantModeOn = false; // #SAFETY#: deceleration unlock const mode

    TargetSpeed = 0;
    TargetPower = 0;
    DriverInfo = "ok.";
    DriverInfoType = INFO_TYPE::STATUS;
    Light = LIGHT::OFF;

    // init state flags
    // #SAFETY#: acceleration lock
    PaddlesJustAdjusted = false;
    AccelerationLocked = true;
  }
  ~CarState(){};
  bool initalize_config();

  // physical car data (measurement values)
  int Speed;        // ADC
  int Acceleration; // ADC Steering Wheel
  int Deceleration; // ADC Steering Wheel
  // #SAFETY#: acceleration lock
  bool AccelerationLocked;  // DSC lock
  bool PaddlesJustAdjusted; // did just padd adjustment: release lock if AccelerationDisplay==0
  int AccelerationDisplay;  // Display Value (-99...+99)
  //#SAFETY-END#

  bool BatteryOn;      // IO-In
  bool PhotoVoltaicOn; // IO-in
  bool MotorOn;        // IO-In
  bool EcoOn;          // IO-In

  PRECHARGE_STATE PrechargeState;    // CAN
  list<BATTERY_ERROR> BatteryErrors; // CAN

  float BatteryVoltage;      // CAN
  float BatteryCurrent;      // CAN
  float PhotoVoltaicCurrent; // ADC
  float MotorVoltage;        // ADC
  float MotorCurrent;        // ADC

  float Mppt1Current; // CAN
  float Mppt2Current; // CAN
  float Mppt3Current; // CAN

  float Umin; // CAN
  float Uavg; // CAN
  float Umax; // CAN

  float T1; // ??
  float T2; // ??
  float T3; // ??
  float T4; // ??

  bool BreakPedal;

  int ReferenceSolarCell;

  // logical car data (values set by driver or chase car)
  DISPLAY_STATUS displayStatus;
  DRIVE_DIRECTION DriveDirection;
  CONSTANT_MODE ConstantMode;
  bool ConstantModeOn; // #SAFETY#: deceleration unlock const mode
  INDICATOR Indicator;
  bool IndicatorBlink;
  bool SdCardDetect;

  float TargetSpeed;
  float TargetRecuperation;
  float TargetPower;
  string DriverInfo;
  SPEED_ARROW SpeedArrow;
  INFO_TYPE DriverInfoType;
  LIGHT Light;

  // All IO pins
  static CarStatePin pins[IOExtPINCOUNT];
  int getIdx(string pinName);
  CarStatePin *getPin(int devNr, int pinNr);
  CarStatePin *getPin(int port);
  CarStatePin *getPin(string pinName);

  std::map<string, int> idxOfPin;
  // std::map<int, Pin> pins; // pins by index

  void init_values();

  // tools
  const string print(string msg, bool withColors = true);
  const string printIOs(string msg, bool withColors = true, bool deltaOnly = false);
  const string serialize(string msg = "");
  const string csv(string msg = "", bool withHeader = false);
  const string batteryErrorsAsString(bool verbose = false);

  // auxiliary variables - read from ser4config.ini
  // [Main]
  string LogFilename; // telemetry data: %stamp% get replaced by datetime stamp if period != 0
  int LogFilePeriod;  // [h], after that a new log file is created, 0 - never
  int LogInterval;    // [s]

  // [TaskTimings]
  int SleepTimeIOExt = 400; // [ms]

  // [PID]
  double Kp = 2;   // proportional
  double Ki = 1;   // integral
  double Kd = 0.1; // differential

  // [Dynamic]
  int PaddleDamping = 10;         // 0...99
  int PaddleOffset = 3000;        // 0 ... 65535: offset when paddle recognize not 0 values
  int PaddleAdjustCounter = 10;   // about seconds§
  float ConstSpeedIncrease = 1.0; // [km/h] per click
  float ConstPowerIncrease = 1.0; // [W] per click

  // [Communication]
  int I2CFrequence = 200;        // [kHz]
  int CarDataLogPeriod = 1000;   // [ms]
  int Serail1Baudrates = 115200; // baud
  int Serial2Baudrate = 9600;    // baud

  // [Telemetry]
  int SendInterval = 1000;    // [ms]
  int MaxCachedRecords = 100; // number of telemetry records hold in cache in case of trasmit errors
};

#endif // CARSTATE_H
