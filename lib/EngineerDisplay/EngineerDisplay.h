//
// Display
//

#ifndef SER_ENGINEER_DISPLAY_H
#define SER_ENGINEER_DISPLAY_H

#include <Display.h>
#include <DisplayValue.h>
#include <Wire.h>
#include <abstract_task.h>
#include <definitions.h>

class EngineerDisplay : public Display {
private:
  //==== Driver Display definitions ==== START
  // display formats and sizes
  int bgColor = ILI9341_LIGHTGREY;

  // PV status [On/Off]
  DisplayValue<bool> PvStatus = DisplayValue<bool>(4, 10, "PV", "", ILI9341_BLUE);
  // MC status [On/Off]
  DisplayValue<bool> McStatus = DisplayValue<bool>(4, 30, "MC", "", ILI9341_BLUE);
  // MPPT 1...3 [A]
  DisplayValue<float> Mppt1 = DisplayValue<float>(168, 10, "MPPT-1", "A");
  DisplayValue<float> Mppt2 = DisplayValue<float>(168, 30, "MPPT-2", "A");
  DisplayValue<float> Mppt3 = DisplayValue<float>(168, 50, "MPPT-3", "A");
  DisplayValue<float> Mppt4 = DisplayValue<float>(168, 70, "MPPT-4", "A");

  // Battery status [OK/Error]
  DisplayValue<string> BatteryStatus = DisplayValue<string>(4, 90, "Battery", "");
  // BMS error [string]
  DisplayValue<string> BmsStatus = DisplayValue<string>(4, 110, "BMS Msg", "");

  // Battery temperature 1...4 [°C]
  DisplayValue<float> Temperature1 = DisplayValue<float>(168, 130, "T-1", "C");
  DisplayValue<float> Temperature2 = DisplayValue<float>(168, 150, "T-2", "C");
  DisplayValue<float> Temperature3 = DisplayValue<float>(168, 170, "T-3", "C");
  DisplayValue<float> Temperature4 = DisplayValue<float>(168, 190, "T-4", "C");
  // Max temperature [°C]
  DisplayValue<float> TemperatureMax = DisplayValue<float>(168, 210, "T-max", "C", ILI9341_DARKCYAN);

  // BMS current [A]
  DisplayValue<float> BatteryCurrent = DisplayValue<float>(4, 130, "BMS I", "A");
  // Voltage battery [V]
  DisplayValue<float> BatteryVoltage = DisplayValue<float>(4, 150, "BMS U", "V");
  // Voltage min, max, average [V]
  DisplayValue<float> VoltageMin = DisplayValue<float>(4, 170, "V-min", "V");
  DisplayValue<float> VoltageAvg = DisplayValue<float>(4, 190, "V-avg", "V");
  DisplayValue<float> VoltageMax = DisplayValue<float>(4, 210, "V-max", "V");
  //==== Driver Display definition ==== END

public:
  string getName(void) { return "EngineerDisplay"; };

  //==== overwrites from base class ==== START
  DISPLAY_STATUS display_setup(DISPLAY_STATUS status) override;
  DISPLAY_STATUS task(DISPLAY_STATUS status, int lifeSignCounter) override;
  //==== overwrites from base class ==== END

  void print(string msg);
  void draw_display_background();
};

#endif // SER_ENGINEER_DISPLAY_H DISPLAY_H
