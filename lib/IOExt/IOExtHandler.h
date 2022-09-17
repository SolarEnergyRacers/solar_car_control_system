/*
 * MCP23017 I/O Extension over I2C  !!! UNTESTED !!!
 */

#ifndef SER_IOEXT_HANDLER_H
#define SER_IOEXT_HANDLER_H

#include <definitions.h>

#include <MCP23017.h>
#include <list>
#include <map>
#include <string>

#include <abstract_task.h>

#include <CarState.h>
#include <CarStatePin.h>
#include <Indicator.h>

// known pin handler
// the handler must its bit copy to oldValue
void batteryOnOffHandler();
void mcOnOffHandler();
void pvOnOffHandler();
void fwdBwdHandler();
void ecoPowerHandler();
void breakPedalHandler();
void indicatorHandler();
void hornHandler();
void nextScreenHandler();
void lightHandler();
void headLightHandler();
void light_switch();
void constantModeHandler();
void constantModeOnHandler();
void constantModeOnSwitch();
void constantModeOffHandler();
void controlModeHandler();
void decreaseHandler();
void increaseHandler();

void paddleAdjustHandler();
void sdCardDetectHandler();

#endif // SER_IOEXT_HANDLER_H
