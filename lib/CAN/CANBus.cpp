//
// CAN Bus
//
#include <definitions.h>

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#include <Arduino.h>
#include <CAN.h>

#include <CarState.h>
#include <CANBus.h>


extern CarState carState;
extern CANBus can;


void onReceiveForwarder(int packetSize) { can.onReceive(packetSize); }

CANBus::CANBus(){
  //init max ages
  this->max_ages[BMS_BASE_ADDR] = MAXAGE_BMU_HEARTBEAT;
  this->max_ages[BMS_BASE_ADDR | 0x1] = MAXAGE_CMU_TEMP; //CMU1
  this->max_ages[BMS_BASE_ADDR | 0x4] = MAXAGE_CMU_TEMP; //CMU2
  this->max_ages[BMS_BASE_ADDR | 0x7] = MAXAGE_CMU_TEMP; //CMU3
  this->max_ages[BMS_BASE_ADDR | 0x2] = MAXAGE_CMU_VOLTAGES; //CMU1
  this->max_ages[BMS_BASE_ADDR | 0x3] = MAXAGE_CMU_VOLTAGES; //CMU1
  this->max_ages[BMS_BASE_ADDR | 0x5] = MAXAGE_CMU_VOLTAGES; //CMU2
  this->max_ages[BMS_BASE_ADDR | 0x6] = MAXAGE_CMU_VOLTAGES; //CMU2
  this->max_ages[BMS_BASE_ADDR | 0x8] = MAXAGE_CMU_VOLTAGES; //CMU3
  this->max_ages[BMS_BASE_ADDR | 0x9] = MAXAGE_CMU_VOLTAGES; //CMU3
  this->max_ages[BMS_BASE_ADDR | 0xF4] = MAXAGE_PACK_SOC;
  this->max_ages[BMS_BASE_ADDR | 0xF5] = MAXAGE_BALANCE_SOC;
  this->max_ages[BMS_BASE_ADDR | 0xF6] = MAXAGE_CHARGER_CONTROL;
  this->max_ages[BMS_BASE_ADDR | 0xF7] = MAXAGE_PRECHARGE_STATUS;
  this->max_ages[BMS_BASE_ADDR | 0xF8] = MAXAGE_MIN_MAX_U_CELL;
  this->max_ages[BMS_BASE_ADDR | 0xF9] = MAXAGE_MIN_MAX_T_CELL;
  this->max_ages[BMS_BASE_ADDR | 0xFA] = MAXAGE_PACK_VOLTAGE;
  this->max_ages[BMS_BASE_ADDR | 0xFB] = MAXAGE_PACK_STATUS;
  this->max_ages[BMS_BASE_ADDR | 0xFC] = MAXAGE_PACK_FAN_STATUS;
  this->max_ages[BMS_BASE_ADDR | 0xFD] = MAXAGE_EXT_PACK_STATUS;

  this->max_ages[MPPT1_BASE_ADDR] = MAXAGE_MPPT_INPUT;
  this->max_ages[MPPT1_BASE_ADDR | 0x1] = MAXAGE_MPPT_OUTPUT;
  this->max_ages[MPPT1_BASE_ADDR | 0x2] = MAXAGE_MPPT_TEMP;
  this->max_ages[MPPT1_BASE_ADDR | 0x3] = MAXAGE_MPPT_AUX_POWER;
  this->max_ages[MPPT1_BASE_ADDR | 0x4] = MAXAGE_MPPT_LIMITS;
  this->max_ages[MPPT1_BASE_ADDR | 0x5] = MAXAGE_MPPT_STATUS;
  this->max_ages[MPPT1_BASE_ADDR | 0x6] = MAXAGE_MPPT_POWER_CONN;

  this->max_ages[MPPT2_BASE_ADDR] = MAXAGE_MPPT_INPUT;
  this->max_ages[MPPT2_BASE_ADDR | 0x1] = MAXAGE_MPPT_OUTPUT;
  this->max_ages[MPPT2_BASE_ADDR | 0x2] = MAXAGE_MPPT_TEMP;
  this->max_ages[MPPT2_BASE_ADDR | 0x3] = MAXAGE_MPPT_AUX_POWER;
  this->max_ages[MPPT2_BASE_ADDR | 0x4] = MAXAGE_MPPT_LIMITS;
  this->max_ages[MPPT2_BASE_ADDR | 0x5] = MAXAGE_MPPT_STATUS;
  this->max_ages[MPPT2_BASE_ADDR | 0x6] = MAXAGE_MPPT_POWER_CONN;

  this->max_ages[MPPT3_BASE_ADDR] = MAXAGE_MPPT_INPUT;
  this->max_ages[MPPT3_BASE_ADDR | 0x1] = MAXAGE_MPPT_OUTPUT;
  this->max_ages[MPPT3_BASE_ADDR | 0x2] = MAXAGE_MPPT_TEMP;
  this->max_ages[MPPT3_BASE_ADDR | 0x3] = MAXAGE_MPPT_AUX_POWER;
  this->max_ages[MPPT3_BASE_ADDR | 0x4] = MAXAGE_MPPT_LIMITS;
  this->max_ages[MPPT3_BASE_ADDR | 0x5] = MAXAGE_MPPT_STATUS;
  this->max_ages[MPPT3_BASE_ADDR | 0x6] = MAXAGE_MPPT_POWER_CONN;


  //init ages
  this->ages[BMS_BASE_ADDR] = INT32_MAX;
  this->ages[BMS_BASE_ADDR | 0x1] = INT32_MAX; //CMU1
  this->ages[BMS_BASE_ADDR | 0x4] = INT32_MAX; //CMU2
  this->ages[BMS_BASE_ADDR | 0x7] = INT32_MAX; //CMU3
  this->ages[BMS_BASE_ADDR | 0x2] = INT32_MAX; //CMU1
  this->ages[BMS_BASE_ADDR | 0x3] = INT32_MAX; //CMU1
  this->ages[BMS_BASE_ADDR | 0x5] = INT32_MAX; //CMU2
  this->ages[BMS_BASE_ADDR | 0x6] = INT32_MAX; //CMU2
  this->ages[BMS_BASE_ADDR | 0x8] = INT32_MAX; //CMU3
  this->ages[BMS_BASE_ADDR | 0x9] = INT32_MAX; //CMU3
  this->ages[BMS_BASE_ADDR | 0xF4] = INT32_MAX;
  this->ages[BMS_BASE_ADDR | 0xF5] = INT32_MAX;
  this->ages[BMS_BASE_ADDR | 0xF6] = INT32_MAX;
  this->ages[BMS_BASE_ADDR | 0xF7] = INT32_MAX;
  this->ages[BMS_BASE_ADDR | 0xF8] = INT32_MAX;
  this->ages[BMS_BASE_ADDR | 0xF9] = INT32_MAX;
  this->ages[BMS_BASE_ADDR | 0xFA] = INT32_MAX;
  this->ages[BMS_BASE_ADDR | 0xFB] = INT32_MAX;
  this->ages[BMS_BASE_ADDR | 0xFC] = INT32_MAX;
  this->ages[BMS_BASE_ADDR | 0xFD] = INT32_MAX;

  this->ages[MPPT1_BASE_ADDR] = INT32_MAX;
  this->ages[MPPT1_BASE_ADDR | 0x1] = INT32_MAX;
  this->ages[MPPT1_BASE_ADDR | 0x2] = INT32_MAX;
  this->ages[MPPT1_BASE_ADDR | 0x3] = INT32_MAX;
  this->ages[MPPT1_BASE_ADDR | 0x4] = INT32_MAX;
  this->ages[MPPT1_BASE_ADDR | 0x5] = INT32_MAX;
  this->ages[MPPT1_BASE_ADDR | 0x6] = INT32_MAX;

  this->ages[MPPT2_BASE_ADDR] = INT32_MAX;
  this->ages[MPPT2_BASE_ADDR | 0x1] = INT32_MAX;
  this->ages[MPPT2_BASE_ADDR | 0x2] = INT32_MAX;
  this->ages[MPPT2_BASE_ADDR | 0x3] = INT32_MAX;
  this->ages[MPPT2_BASE_ADDR | 0x4] = INT32_MAX;
  this->ages[MPPT2_BASE_ADDR | 0x5] = INT32_MAX;
  this->ages[MPPT2_BASE_ADDR | 0x6] = INT32_MAX;

  this->ages[MPPT3_BASE_ADDR] = INT32_MAX;
  this->ages[MPPT3_BASE_ADDR | 0x1] = INT32_MAX;
  this->ages[MPPT3_BASE_ADDR | 0x2] = INT32_MAX;
  this->ages[MPPT3_BASE_ADDR | 0x3] = INT32_MAX;
  this->ages[MPPT3_BASE_ADDR | 0x4] = INT32_MAX;
  this->ages[MPPT3_BASE_ADDR | 0x5] = INT32_MAX;
  this->ages[MPPT3_BASE_ADDR | 0x6] = INT32_MAX;

  
}

string CANBus::getName() { return "CAN_Task"; }

void CANBus::re_init() {
  CAN.end();
  CANBus::init();
}

void CANBus::init() {
  mutex = xSemaphoreCreateBinary();

  CAN.setPins(CAN_RX, CAN_TX);
  CAN.onReceive(onReceiveForwarder); // couldn't get it to work with method of object

  if (!CAN.begin(CAN_SPEED)) {
    // opening CAN failed :,(
  }

  xSemaphoreGive(mutex);
}

void CANBus::exit() {
  // Exit needs to be implemented for Task, here or in abstract_task
  CAN.end();
}

void CANBus::task() {
  CANPacket packet;

  while (1) {

    while (this->rxBuffer.isAvailable()) {
      packet = this->rxBuffer.pop();
      
      /*
      printf("----------------\n");
      printf("id: %X\n", packet.getID());
      printf("data 32 0: %X\n", packet.getData_ui32(0));
      printf("data 32 1: %X\n", packet.getData_ui32(1));
      */

      // Do something with packet
      switch (packet.getID()) {
      case BMS_BASE_ADDR:
        // heartbeat packet.getData_ui32(0)
        break;
      case BMS_BASE_ADDR | 0xFA:
        carState.BatteryVoltage = (float)packet.getData_ui32(0) / 1000.0;
        carState.Uavg = carState.BatteryVoltage / 30; //
        carState.BatteryCurrent = (float)packet.getData_i32(1) / 1000.0;
        // Battery Voltage mV packet.getData_ui32(0)
        // Battery Current mA packet.getData_i32(1)
        break;
      case BMS_BASE_ADDR | 0xF8:
        carState.Umin = packet.getData_ui16(0);
        carState.Umax = packet.getData_ui16(1);
        // Battery min Cell Voltage mV packet.getData_ui16(0)
        // Battery max Cell Voltage mV packet.getData_ui16(1)
        // CMU number with min Cell Voltage packet.getData_ui8(4)
        // Cell number with min Voltage packet.getData_ui8(5)
        // CMU number with max Cell Voltage packet.getData_ui8(6)
        // Cell number with max Voltage packet.getData_ui8(7)
        break;
      case BMS_BASE_ADDR | 0xF7:
        // Contactor 1 driver error packet.getData_b(0)
        // Contactor 2 driver error packet.getData_b(1)
        // Contactor 1 driver output status packet.getData_b(2)
        // Contactor 2 driver output status packet.getData_b(3)
        // 12V contactor supply OK packet.getData_b(4)
        // Contactor 3 driver error packet.getData_b(5)
        // Contactor 3 driver output status packet.getData_b(6)

        // Precharge State packet.getData_ui8(1)
        /*
            0=Error
            1=Idle
            2=Measure
            3=Precharge
            4=Run
            5=Enable Pack
        */
        switch(packet.getData_ui8(1)){
          case 0:
            carState.PrechargeState = PRECHARGE_STATE::ERROR;
            break;
          case 1:
            carState.PrechargeState = PRECHARGE_STATE::IDLE;
            break;
          case 2:
            carState.PrechargeState = PRECHARGE_STATE::MEASURE;
            break;
          case 3:
            carState.PrechargeState = PRECHARGE_STATE::PRECHARGE;
            break;
          case 4:
            carState.PrechargeState = PRECHARGE_STATE::RUN;
            break;
          case 5:
            carState.PrechargeState = PRECHARGE_STATE::ENABLE_PACK;
            break;
        }
    

        // Precharge Timer info also available
        break;
      case BMS_BASE_ADDR | 0xFD:
        carState.BatteryErrors.clear();
        
        if(packet.getData_ui32(0) > 0){ // Saving CPU time in case there are no errors
          for(int i = 0; i < 13; i++){
            if(packet.getData_b(i)){
              carState.BatteryErrors.push_front(static_cast<BATTERY_ERROR>(i));
            }
          }
        }
        // Cell over voltage packet.getData_b(0)
        // Cell under voltage packet.getData_b(1)
        // Cell over Temp packet.getData_b(2)
        // Measurement untrusted packet.getData_b(3)

        // CMU Comm Timeout packet.getData_b(4)
        // Vehicle Comm Timeout packet.getData_b(5)
        // BMU in Setup Mode packet.getData_b(6)
        // CMU CAN Power Status packet.getData_b(7)
      
        // Pack isolation test fail packet.getData_b(8)
        // SOC measurement invalid packet.getData_b(9)
        // CAN 12V in low, about to shutdown packet.getData_b(10)
        // Contactor Stuck/Not engaged packet.getData_b(11)

        // Extra Cell present packet.getData_b(12)
        break;
      case MPPT1_BASE_ADDR | 0x1:
        carState.Mppt1Current = packet.getData_f32(1);
        //carState.PhotoVoltaicCurrent = carState.Mppt1Current + carState.Mppt2Current + carState.Mppt3Current;

        // MPPT1 Output Voltage V packet.getData_f32(0)
        // MPPT1 Output Current A packet.getData_f32(1)
        break;
      case MPPT2_BASE_ADDR | 0x1:
        carState.Mppt2Current = packet.getData_f32(1);
        //carState.PhotoVoltaicCurrent = carState.Mppt1Current + carState.Mppt2Current + carState.Mppt3Current;

        // MPPT2 Output Voltage V packet.getData_f32(0)
        // MPPT2 Output Current A packet.getData_f32(1)
        break;
      case MPPT3_BASE_ADDR | 0x1:
        carState.Mppt3Current = packet.getData_f32(1);
        //carState.PhotoVoltaicCurrent = carState.Mppt1Current + carState.Mppt2Current + carState.Mppt3Current;

        // MPPT3 Output Voltage V packet.getData_f32(0)
        // MPPT3 Output Current A packet.getData_f32(1)
        break;
      }
    }
    this->sleep(CAN_TASK_WAIT);
  }
}

void CANBus::onReceive(int packetSize) {

  CANPacket packet;
  uint64_t rxData = 0;

  packet.setID(CAN.packetId());

  if(this->max_ages[packet.getID()] == 0 || (this->max_ages[packet.getID()] != -1 && millis() - this->ages[packet.getID()] > this->max_ages[packet.getID()])){
    
    this->ages[packet.getID()] = millis();

    for (int i = 0; i < packetSize; i++) {
      if (CAN.available()) {
        rxData = rxData | (((uint64_t)CAN.read()) << (i * 8));
      }
    }

    packet.setData(rxData);

    // Add packet to buffer so task can handle it later
    rxBuffer.push(packet);
  }
}