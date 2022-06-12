//
// MCP23017 I/O Extension over I2C
//
#include <definitions.h>

#include <stdio.h>

// standard libraries
#include <fmt/core.h>
#include <inttypes.h>
#include <iostream>
#include <stdio.h>
#include <string>

#include <MCP23017.h> // MCP23017
#include <Wire.h>     // I2C

#include <CarControl.h>
#include <Console.h>
#include <DriverDisplay.h>
#include <EngineerDisplay.h>
#include <Helper.h>
#include <I2CBus.h>
#include <IOExt.h>
#include <IOExtHandler.h>

extern Console console;
extern I2CBus i2cBus;
extern Indicator indicator;
extern IOExt ioExt;
extern CarState carState;
extern CarControl carControl;
extern bool systemOk;

// ------------------
// FreeRTOS functions

volatile bool ioInterruptRequest;
void IRAM_ATTR ioExt_interrupt_handler() { ioInterruptRequest = true; };

string IOExt::re_init() { return init(); }

string IOExt::init() {
  bool hasError = false;
  console << "[  ] Init IOExt devices...\n";
  for (int devNr = 0; devNr < MCP23017_NUM_DEVICES; devNr++) {
    console << "     DeviceNr [" << devNr << "]:\n";
    xSemaphoreTakeT(i2cBus.mutex);
    ioExt.IOExtDevs[devNr].init();
    ioExt.IOExtDevs[devNr].interruptMode(MCP23017InterruptMode::Or);
    ioExt.IOExtDevs[devNr].interrupt(MCP23017Port::A, CHANGE); // FALLING);
    ioExt.IOExtDevs[devNr].interrupt(MCP23017Port::B, CHANGE); // FALLING);
    xSemaphoreGive(i2cBus.mutex);
    for (int pinNr = 0; pinNr < MCP23017_NUM_PORTS; pinNr++) {
      CarStatePin *pin = carState.getPin(devNr, pinNr);
      carState.idxOfPin.insert(pair<string, int>{pin->name, getIdx(devNr, pinNr)});
      xSemaphoreTakeT(i2cBus.mutex);
      ioExt.IOExtDevs[devNr].pinMode(pinNr, pin->mode);
      // if (pin->mode != OUTPUT) {
      //   pin->value = pin->oldValue = ioExt.IOExtDevs[devNr].digitalRead(pinNr);
      // }
      xSemaphoreGive(i2cBus.mutex);
      int dev = pin->port >> 4;
      int devPin = pin->port & 0xf;
      console << fmt::format("        {:#04x} [{:02d}] dev:{:02d}, devpin:{:02d}, mode:{}, value={} ({})\n", pin->port,
                             carState.getIdx(pin->name), dev, devPin, pin->mode != OUTPUT ? "INPUT " : "OUTPUT", pin->value,
                             pin->name.c_str());
    }
    xSemaphoreTakeT(i2cBus.mutex);
    ioExt.IOExtDevs[devNr].clearInterrupts();
    xSemaphoreGive(i2cBus.mutex);
    console << "     ok " << getName() << "[" << devNr << "]\n";
  }
  ioInterruptRequest = false;
  pinMode(I2C_INTERRUPT, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(I2C_INTERRUPT), ioExt_interrupt_handler, CHANGE);
  return fmt::format("[{}] IOExt initialized.", hasError ? "--" : "ok");
}

void IOExt::exit(void) {
  // TODO
}
// ------------------

// void IOExt::handleIoInterrupt() {
//   if (isInInterruptHandler) {
//     // console << fmt::format("Jump out of interrupt handler at {}\n", millis());
//     return;
//   }
//   isInInterruptHandler = true;
//   // console << fmt::format("Do interrupt handler at {}}\n", millis());
//   readPins();
//   //readAll(true, false, "", false);
//   // console << fmt::format("Clear interrupts   {}\n", millis());
//   xSemaphoreTakeT(i2cBus.mutex);
//   for (int devNr = 0; devNr < MCP23017_NUM_DEVICES; devNr++) {
//     uint8_t portA, portB;
//     // uint8_t allPins = ioExt.IOExtDevs[devNr].read();
//     ioExt.IOExtDevs[devNr].clearInterrupts(portA, portB);
//     // console << fmt::format("Jump in interrupt handler at {:ld}::\tdevNr: {} -- pA {:02x}, pB {:02x}\n", millis(), devNr, portA,
//     portB); ioExt.IOExtDevs[devNr].clearInterrupts();
//   }
//   xSemaphoreGive(i2cBus.mutex);
//   isInInterruptHandler = false;
// }

// void IOExt::readAll(bool deltaOnly, bool forced, string indent, bool verbose) {
//   const string normalColor = "\033[0;39m";
//   const string highLightColorChg = "\033[1;36m"; // blue
//   list<void (*)()> pinHandlerList;
//   stringstream ss("");
//   // xSemaphoreTakeT(i2cBus.mutex);
//   for (int devNr = 0; devNr < MCP23017_NUM_DEVICES; devNr++) {
//     for (int pinNr = 0; pinNr < MCP23017_NUM_PORTS; pinNr++) {
//       // console << "readAll dev:" << devNr << ", pinNr:" << pinNr << "\n";
//       CarStatePin *pin = carState.getPin(devNr, pinNr);
//       if (pin->mode != OUTPUT) {

//         xSemaphoreTakeT(i2cBus.mutex);
//         pin->value = ioExt.IOExtDevs[devNr].digitalRead(pinNr);
//         xSemaphoreGive(i2cBus.mutex);
//         if (verbose) {
//           ss << normalColor << indent;
//           ss << fmt::format("Port {:02x} DevNr {} PinNr {:x} Value={}   {}", pin->port, devNr, pinNr, pin->value, pin->name);
//           if (pin->value != pin->oldValue)
//             ss << " *";
//           ss << "\n";
//         }
//         console << ":";
//         if (pin->handlerFunction != NULL && (pin->value != pin->oldValue || !pin->inited || forced)) {
//           console << "\n"
//                   << pin->name << ": " << pin->oldValue << " -- " << pin->value << ", inited:" << pin->inited << ", (forced:" << forced
//                   << ")"
//                   << "\n";
//           pin->inited = true;
//           pinHandlerList.push_back(pin->handlerFunction);
//           pin->oldValue = pin->value;
//         }
//       }
//     }
//   }
//   // xSemaphoreGive(i2cBus.mutex);
//   if (verbose) {
//     console << ss.str();
//   }
//   if (verboseMode) {
//     string outString = carState.printIOs("", true, deltaOnly);
//     if (outString.length() > 0)
//       console << indent << outString << "\n";
//   }
//   // avoid multi registration:
//   pinHandlerList.unique();
//   // call all handlers for changed pins

//   for (void (*pinHandler)() : pinHandlerList) {
//     pinHandler();
//   }
//   pinHandlerList.clear();
// }

void IOExt::setPortMode(int port, uint8_t mode) {
  int devNr = port >> 4;
  int pin = port & 0xf;
  xSemaphoreTakeT(i2cBus.mutex);
  ioExt.IOExtDevs[devNr].pinMode(pin, mode);
  xSemaphoreGive(i2cBus.mutex);
}

void IOExt::setPort(int port, bool value) {
  int devNr = port >> 4;
  int pin = port & 0xf;
  xSemaphoreTakeT(i2cBus.mutex);
  ioExt.IOExtDevs[devNr].digitalWrite(pin, value ? 1 : 0);
  xSemaphoreGive(i2cBus.mutex);
  if (verboseMode) {
    console << fmt::format("Set BOOL -- port:0x{:02x}--devNr: {}--pin: {}--value= {}  <-- {}  ---{}-\n", port, devNr, pin, (value ? 1 : 0),
                           carState.getPin(devNr, pin)->name, millis());
  }
}

int IOExt::getPort(int port) {
  int devNr = port >> 4;
  int pin = port & 0xf;
  xSemaphoreTakeT(i2cBus.mutex);
  int value = ioExt.IOExtDevs[devNr].digitalRead(pin);
  xSemaphoreGive(i2cBus.mutex);
  // console << fmt::format("Get BOOL--port:0x{:02x)--devNr:{}--pin:{}--value:{}--{}-\n", port, devNr, pin, value, millis());
  return value;
}

bool IOExt::readPins(PinReadMode type) {
  if (!isInInputHandler) {
    isInInputHandler = true;
    bool hasChanges = false;
    for (auto &pin : carState.pins) {
      if (pin.mode != OUTPUT) {
        pin.value = getPort(pin.port);
        if (pin.oldValue != pin.value || type == PinReadMode::ALL) {
          console << "Get BOOL -- " << pin.oldValue << " --> " << pin.value << ", inited:" << pin.inited << " <-- " << pin.name << "\t("<< millis()<<")\n";
          if (pin.oldValue != pin.value)
            hasChanges = true;
        }
      }
    }
    isInInputHandler = false;
    return hasChanges;
  }
  return false;
}

bool IOExt::readAndHandlePins(PinHandleMode mode) {
  if (!isInInputHandler) {
    isInInputHandler = true;
    bool hasChanges = false;
    list<void (*)()> pinHandlerList;
    for (auto &pin : carState.pins) {
      if (pin.mode != OUTPUT) {
        pin.inited = true;
        pin.value = getPort(pin.port);
        unsigned long timestamp = millis();
        if (pin.handlerFunction != NULL && (pin.oldValue != pin.value || !pin.inited || mode == PinHandleMode::FORCED)) {
          console << "Get BOOL -- " << pin.oldValue << " --> " << pin.value << ", inited:" << pin.inited << " <-- " << pin.name
                  << "\t -> handle"<< "\t("<< timestamp - pin.timestamp<<")\n";
          pin.inited = true;
          pinHandlerList.push_back(pin.handlerFunction);
          pin.oldValue = pin.value;
          pin.timestamp = timestamp;
          hasChanges = true;
        }
      }
    }
    // avoid multi registration:
    pinHandlerList.unique();
    // call all handlers for changed pins
    for (void (*pinHandler)() : pinHandlerList) {
      pinHandler();
    }
    pinHandlerList.clear();
    isInInputHandler = false;
    return hasChanges;
  }
  return false;
}

void IOExt::task() {

  // polling loop
  while (1) {
    if (systemOk) {
      bool changedInputs = readAndHandlePins();
      if (changedInputs) {
        carControl.valueChangedHandler();
      }
      // readAll(true, false, "", false);
      //   handle INPUT pin interrupts
      //   if (ioInterruptRequest) {
      //     console << "jump ioInterruptRequest " << millis() << "\n";
      //     handleIoInterrupt();
      //     ioInterruptRequest = false;
      //     // carControl.valueChangedHandler();
      //   }

      //  update OUTPUT pins
      for (auto &pin : carState.pins) {
        if (pin.mode == OUTPUT && (pin.oldValue != pin.value || !pin.inited)) {
          // console << pin.name << ": " << pin.oldValue << " -- " << pin.value << ", inited:" << pin.inited << "\n";
          pin.oldValue = pin.value;
          pin.inited = true;
          setPort(pin.port, pin.value);
        }
      }
    }
    vTaskDelay(sleep_polling_ms / portTICK_PERIOD_MS);
  }
}
