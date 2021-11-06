//
// Display
//

#ifndef SER_DISPLAY_C_H
#define SER_DISPLAY_C_H

#define ILI9341 // (320x240)

#include <ADS1X15.h>
#include <Adafruit_ILI9341.h> // placed here for display colors in other moduls

#include <CarState.h>
#include <LocalFunctionsAndDevices.h>
#include <abstract_task.h>

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

using namespace std;

// namespace Display {

enum class DISPLAY_STATUS {
  SETUPDRIVER,
  SETUPENGINEER,
  CONSOLE,
  DEMOSCREEN,
  BACKGROUNDDRIVER,
  BACKGROUNDENGINEER,
  DRIVER,
  ENGINEER,
  HALTED
};
static const char *DISPLAY_STATUS_str[] = {"SETUPDRIVER",        "SETUPENGINEER", "CONSOLE",  "DEMOSCREEN", "BACKGROUNDDRIVER",
                                           "BACKGROUNDENGINEER", "DRIVER",        "ENGINEER", "HALTED"};

template <typename Enumeration> auto as_integer(Enumeration const value) -> typename std::underlying_type<Enumeration>::type {
  return static_cast<typename std::underlying_type<Enumeration>::type>(value);
}

class Display : public abstract_task {

private:
  //==== Display definitions ==== START
  uint32_t sleep_polling_ms = 50;

  //==== Display definitions ==== END

  void lifeSign();
  void setupScrollArea(uint16_t TFA, uint16_t BFA);
  void scrollAddress(uint16_t VSP);
  int scroll(int lines);
  void _setup(void);

protected:
  int bgColor = ILI9341_WHITE;
  Adafruit_ILI9341 tft = Adafruit_ILI9341(0, 0, 0, 0, 0, 0);
  volatile DISPLAY_STATUS status;
  bool _is_ready() {
    bool isReady = (status == DISPLAY_STATUS::CONSOLE || status == DISPLAY_STATUS::DRIVER || status == DISPLAY_STATUS::ENGINEER);
    return isReady;
  }

public:
  virtual ~Display() {tft = Adafruit_ILI9341(0, 0, 0, 0, 0, 0);}
  Display() { status = DISPLAY_STATUS::HALTED; }

  // RTOS task
  void init(void);
  void re_init(void);
  void exit(void);
  void set_DisplayStatus(DISPLAY_STATUS theNewStatus) { status = theNewStatus; };
  DISPLAY_STATUS get_DisplayStatus() { return status; };
  char * get_DisplayStatus_text() { return (char *)DISPLAY_STATUS_str[(int)status]; };

  void print(string msg);
  void clear_screen(int bgColor);
  int getPixelWidthOfTexts(int textSize, string t1, string t2);

  // internal functions for inner task communication
  void task(void);

  // handler called for inherited classes
  virtual string getName(void);
  virtual DISPLAY_STATUS display_setup(DISPLAY_STATUS status) { return DISPLAY_STATUS::HALTED; };
  virtual DISPLAY_STATUS task(DISPLAY_STATUS status, int lifeSignCounter) { return DISPLAY_STATUS::HALTED; };

  // workers
  float write_float(int x, int y, float valueLast, float value, int textSize, int color);
  int write_ganz_99(int x, int y, int valueLast, int value, int textSize, int color);
  int write_nat_999(int x, int y, int valueLast, int value, int textSize, int color);

  void drawCentreString(const string &buf, int x, int y);
  int getColorForInfoType(INFO_TYPE type);
  bool init_display(void);
};
//} // namespace Display
#endif // SER_DISPLAY_C_H
