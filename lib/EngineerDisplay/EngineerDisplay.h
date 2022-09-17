//
// EngineeringDisplay
//

#ifndef SER_ENGINEER_DISPLAY_H
#define SER_ENGINEER_DISPLAY_H

#include <Display.h>
#include <DisplayValue.h>
#include <Wire.h>
#include <abstract_task.h>
#include <definitions.h>

class EngineerDisplay : public Display {
public:
  EngineerDisplay() { bgColor = ILI9341_ORANGE; };
  ~EngineerDisplay(){};
  //==== overwrites from base class ==== START
  string getName(void) { return "EngineerDisplay"; };

protected:
  //==== overwrites from base class ==== START
  DISPLAY_STATUS display_setup() override;
  DISPLAY_STATUS task(int lifeSignCounter) override;
  //==== overwrites from base class ==== END

private:
  //==== display cache =====================
  // ... to avoid flickering
  bool justInited;
  //=======================================

  //==== Driver Display definitions ==== START
  // Stati [On/Off]
  DisplayValue<bool> MotorOn = DisplayValue<bool>(4, 10, "MC   :", "%3s", "", ILI9341_BLUE);
  DisplayValue<bool> BatteryOn = DisplayValue<bool>(4, 30, "Bat  :", "%3s", "", ILI9341_BLUE);
  DisplayValue<bool> PhotoVoltaicOn = DisplayValue<bool>(4, 50, "PV   :", "%3s", "", ILI9341_BLUE);

  // MPPTs 1...3 [A]
  DisplayValue<float> Mppt1 = DisplayValue<float>(160, 10, "MPPT-1:", "%5.2f", "A");
  DisplayValue<float> Mppt2 = DisplayValue<float>(160, 30, "MPPT-2:", "%5.2f", "A");
  DisplayValue<float> Mppt3 = DisplayValue<float>(160, 50, "MPPT-3:", "%5.2f", "A");
  // DisplayValue<float> Mppt4 = DisplayValue<float>(160, 70, "MPPT-4:", "%5.2f", "A");

  // Battery status [OK/Error]
  DisplayValue<string> BatteryStatus = DisplayValue<string>(4, 90, "Battery:", "%18s", "");
  // BMS error [string]
  DisplayValue<string> BmsStatus = DisplayValue<string>(4, 110, "BMS Msg:", "%18s", "");

  // MPPT1...3 temperature [°C]
  DisplayValue<float> Temperature1 = DisplayValue<float>(168, 130, "MPPT-1:", "%4.1f", "C");
  DisplayValue<float> Temperature2 = DisplayValue<float>(168, 150, "MPPT-2:", "%4.1f", "C");
  DisplayValue<float> Temperature3 = DisplayValue<float>(168, 170, "MPPT-3:", "%4.1f", "C");
  // Motorcontroller temperature [°C]
  //  DisplayValue<float> Temperature4 = DisplayValue<float>(168, 190, "T-4   :", "%4.1f", "C");
  // Min and max BMS cell temperature [°C]
  DisplayValue<float> TemperatureMin = DisplayValue<float>(168, 190, "BATmin:", "%4.1f", "C", ILI9341_DARKCYAN);
  DisplayValue<float> TemperatureMax = DisplayValue<float>(168, 210, "BATmax:", "%4.1f", "C", ILI9341_DARKCYAN);

  // BMS current [A]
  DisplayValue<float> BatteryCurrent = DisplayValue<float>(4, 130, "BMS I:", "%5.2f", "A");
  // Voltage battery [V]
  DisplayValue<float> BatteryVoltage = DisplayValue<float>(4, 150, "BMS U:", "%5.1f", "V");
  // Voltage min, max, average [V]
  DisplayValue<float> VoltageMin = DisplayValue<float>(4, 170, "U-min:", "%5.3f", "V");
  DisplayValue<float> VoltageAvg = DisplayValue<float>(4, 190, "U-avg:", "%5.3f", "V");
  DisplayValue<float> VoltageMax = DisplayValue<float>(4, 210, "U-max:", "%5.3f", "V");
  //==== Driver Display definition ==buf[== END

  void draw_display_background();
};

#endif // SER_ENGINEER_DISPLAY_H DISPLAY_H
