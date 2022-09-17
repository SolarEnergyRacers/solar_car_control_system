//
// CAN Bus
//
#include <fmt/core.h>

#include <definitions.h>

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#include <Arduino.h>
#include <CAN.h>

#include <CANBus.h>
#include <CarState.h>
#include <Console.h>
#include <Helper.h>

extern CarState carState;
extern CANBus can;
extern Console console;

void onReceiveForwarder(int packetSize) { can.onReceive(packetSize); }

CANBus::CANBus() {
  // init max ages
  max_ages[BMS_BASE_ADDR] = MAXAGE_BMU_HEARTBEAT;
  max_ages[BMS_BASE_ADDR | 0x1] = MAXAGE_CMU_TEMP;     // CMU1
  max_ages[BMS_BASE_ADDR | 0x4] = MAXAGE_CMU_TEMP;     // CMU2
  max_ages[BMS_BASE_ADDR | 0x7] = MAXAGE_CMU_TEMP;     // CMU3
  max_ages[BMS_BASE_ADDR | 0x2] = MAXAGE_CMU_VOLTAGES; // CMU1
  max_ages[BMS_BASE_ADDR | 0x3] = MAXAGE_CMU_VOLTAGES; // CMU1
  max_ages[BMS_BASE_ADDR | 0x5] = MAXAGE_CMU_VOLTAGES; // CMU2
  max_ages[BMS_BASE_ADDR | 0x6] = MAXAGE_CMU_VOLTAGES; // CMU2
  max_ages[BMS_BASE_ADDR | 0x8] = MAXAGE_CMU_VOLTAGES; // CMU3
  max_ages[BMS_BASE_ADDR | 0x9] = MAXAGE_CMU_VOLTAGES; // CMU3
  max_ages[BMS_BASE_ADDR | 0xF4] = MAXAGE_PACK_SOC;
  max_ages[BMS_BASE_ADDR | 0xF5] = MAXAGE_BALANCE_SOC;
  max_ages[BMS_BASE_ADDR | 0xF6] = MAXAGE_CHARGER_CONTROL;
  max_ages[BMS_BASE_ADDR | 0xF7] = MAXAGE_PRECHARGE_STATUS;
  max_ages[BMS_BASE_ADDR | 0xF8] = MAXAGE_MIN_MAX_U_CELL;
  max_ages[BMS_BASE_ADDR | 0xF9] = MAXAGE_MIN_MAX_T_CELL;
  max_ages[BMS_BASE_ADDR | 0xFA] = MAXAGE_PACK_VOLTAGE;
  max_ages[BMS_BASE_ADDR | 0xFB] = MAXAGE_PACK_STATUS;
  max_ages[BMS_BASE_ADDR | 0xFC] = MAXAGE_PACK_FAN_STATUS;
  max_ages[BMS_BASE_ADDR | 0xFD] = MAXAGE_EXT_PACK_STATUS;

  max_ages[MPPT1_BASE_ADDR] = MAXAGE_MPPT_INPUT;
  max_ages[MPPT1_BASE_ADDR | 0x1] = MAXAGE_MPPT_OUTPUT;
  max_ages[MPPT1_BASE_ADDR | 0x2] = MAXAGE_MPPT_TEMP;
  max_ages[MPPT1_BASE_ADDR | 0x3] = MAXAGE_MPPT_AUX_POWER;
  max_ages[MPPT1_BASE_ADDR | 0x4] = MAXAGE_MPPT_LIMITS;
  max_ages[MPPT1_BASE_ADDR | 0x5] = MAXAGE_MPPT_STATUS;
  max_ages[MPPT1_BASE_ADDR | 0x6] = MAXAGE_MPPT_POWER_CONN;

  max_ages[MPPT2_BASE_ADDR] = MAXAGE_MPPT_INPUT;
  max_ages[MPPT2_BASE_ADDR | 0x1] = MAXAGE_MPPT_OUTPUT;
  max_ages[MPPT2_BASE_ADDR | 0x2] = MAXAGE_MPPT_TEMP;
  max_ages[MPPT2_BASE_ADDR | 0x3] = MAXAGE_MPPT_AUX_POWER;
  max_ages[MPPT2_BASE_ADDR | 0x4] = MAXAGE_MPPT_LIMITS;
  max_ages[MPPT2_BASE_ADDR | 0x5] = MAXAGE_MPPT_STATUS;
  max_ages[MPPT2_BASE_ADDR | 0x6] = MAXAGE_MPPT_POWER_CONN;

  max_ages[MPPT3_BASE_ADDR] = MAXAGE_MPPT_INPUT;
  max_ages[MPPT3_BASE_ADDR | 0x1] = MAXAGE_MPPT_OUTPUT;
  max_ages[MPPT3_BASE_ADDR | 0x2] = MAXAGE_MPPT_TEMP;
  max_ages[MPPT3_BASE_ADDR | 0x3] = MAXAGE_MPPT_AUX_POWER;
  max_ages[MPPT3_BASE_ADDR | 0x4] = MAXAGE_MPPT_LIMITS;
  max_ages[MPPT3_BASE_ADDR | 0x5] = MAXAGE_MPPT_STATUS;
  max_ages[MPPT3_BASE_ADDR | 0x6] = MAXAGE_MPPT_POWER_CONN;

  // init ages
  ages[BMS_BASE_ADDR] = INT32_MAX;
  ages[BMS_BASE_ADDR | 0x1] = INT32_MAX; // CMU1
  ages[BMS_BASE_ADDR | 0x4] = INT32_MAX; // CMU2
  ages[BMS_BASE_ADDR | 0x7] = INT32_MAX; // CMU3
  ages[BMS_BASE_ADDR | 0x2] = INT32_MAX; // CMU1
  ages[BMS_BASE_ADDR | 0x3] = INT32_MAX; // CMU1
  ages[BMS_BASE_ADDR | 0x5] = INT32_MAX; // CMU2
  ages[BMS_BASE_ADDR | 0x6] = INT32_MAX; // CMU2
  ages[BMS_BASE_ADDR | 0x8] = INT32_MAX; // CMU3
  ages[BMS_BASE_ADDR | 0x9] = INT32_MAX; // CMU3
  ages[BMS_BASE_ADDR | 0xF4] = INT32_MAX;
  ages[BMS_BASE_ADDR | 0xF5] = INT32_MAX;
  ages[BMS_BASE_ADDR | 0xF6] = INT32_MAX;
  ages[BMS_BASE_ADDR | 0xF7] = INT32_MAX;
  ages[BMS_BASE_ADDR | 0xF8] = INT32_MAX;
  ages[BMS_BASE_ADDR | 0xF9] = INT32_MAX;
  ages[BMS_BASE_ADDR | 0xFA] = INT32_MAX;
  ages[BMS_BASE_ADDR | 0xFB] = INT32_MAX;
  ages[BMS_BASE_ADDR | 0xFC] = INT32_MAX;
  ages[BMS_BASE_ADDR | 0xFD] = INT32_MAX;

  ages[MPPT1_BASE_ADDR] = INT32_MAX;
  ages[MPPT1_BASE_ADDR | 0x1] = INT32_MAX;
  ages[MPPT1_BASE_ADDR | 0x2] = INT32_MAX;
  ages[MPPT1_BASE_ADDR | 0x3] = INT32_MAX;
  ages[MPPT1_BASE_ADDR | 0x4] = INT32_MAX;
  ages[MPPT1_BASE_ADDR | 0x5] = INT32_MAX;
  ages[MPPT1_BASE_ADDR | 0x6] = INT32_MAX;

  ages[MPPT2_BASE_ADDR] = INT32_MAX;
  ages[MPPT2_BASE_ADDR | 0x1] = INT32_MAX;
  ages[MPPT2_BASE_ADDR | 0x2] = INT32_MAX;
  ages[MPPT2_BASE_ADDR | 0x3] = INT32_MAX;
  ages[MPPT2_BASE_ADDR | 0x4] = INT32_MAX;
  ages[MPPT2_BASE_ADDR | 0x5] = INT32_MAX;
  ages[MPPT2_BASE_ADDR | 0x6] = INT32_MAX;

  ages[MPPT3_BASE_ADDR] = INT32_MAX;
  ages[MPPT3_BASE_ADDR | 0x1] = INT32_MAX;
  ages[MPPT3_BASE_ADDR | 0x2] = INT32_MAX;
  ages[MPPT3_BASE_ADDR | 0x3] = INT32_MAX;
  ages[MPPT3_BASE_ADDR | 0x4] = INT32_MAX;
  ages[MPPT3_BASE_ADDR | 0x5] = INT32_MAX;
  ages[MPPT3_BASE_ADDR | 0x6] = INT32_MAX;
}

string CANBus::re_init() {
  CAN.end();
  return CANBus::init();
}

string CANBus::init() {
  bool hasError = false;
  console << "[  ] Init CANBus...\n";
  mutex = xSemaphoreCreateBinary();
  CAN.setPins(CAN_RX, CAN_TX);
  CAN.onReceive(onReceiveForwarder); // couldn't get it to work with method of object

  if (!CAN.begin(CAN_SPEED)) {
    xSemaphoreGive(mutex);
    // opening CAN failed :,(
    console << fmt::format("     ERROR: CANBus with rx={:02x}, tx={:02x} NOT inited.\n", CAN_RX, CAN_TX);
    hasError = true;
  } else {
    xSemaphoreGive(mutex);
    console << fmt::format("     CANBus with rx={:02x}, tx={:02x} inited.\n", CAN_RX, CAN_TX);
  }

  return fmt::format("[{}] CANBus initialized.", hasError ? "--" : "ok");
}

void CANBus::exit() {
  // Exit needs to be implemented for Task, here or in abstract_task
  xSemaphoreGive(mutex);
  CAN.end();
  xSemaphoreGive(mutex);
}

void CANBus::task() {
  CANPacket packet;

  while (1) {

    while (this->rxBuffer.isAvailable()) {
      packet = this->rxBuffer.pop();

      // Do something with packet
      switch (packet.getID()) {
      case BMS_BASE_ADDR:
        // heartbeat packet.getData_ui32(0)
        break;
      case BMS_BASE_ADDR | 0xFA:
        carState.BatteryVoltage = (float)packet.getData_ui32(0) / 1000.0;
        carState.Uavg = carState.BatteryVoltage / 28.; // 28 cells in serie
        carState.BatteryCurrent = (float)packet.getData_i32(1) / 1000.0;
        // Battery Voltage mV packet.getData_ui32(0)
        // Battery Current mA packet.getData_i32(1)
        if (verboseModeCan) {
          console << ", Uavg=" << carState.Uavg << ", BatVolt=" << carState.BatteryVoltage << ", BatCur=" << carState.BatteryCurrent
                  << fmt::format(", raw: 0x{:08x}", packet.getData_ui64()) << "\n";
        }
        break;
      case BMS_BASE_ADDR | 0xF8:
        carState.Umin = packet.getData_ui16(0) / 1000.;
        carState.Umax = packet.getData_ui16(1) / 1000.;
        // Battery min Cell Voltage mV packet.getData_ui16(0)
        // Battery max Cell Voltage mV packet.getData_ui16(1)
        // CMU number with min Cell Voltage packet.getData_ui8(4)
        // Cell number with min Voltage packet.getData_ui8(5)
        // CMU number with max Cell Voltage packet.getData_ui8(6)
        // Cell number with max Voltage packet.getData_ui8(7)
        if (verboseModeCan) {
          console << ", Umin=" << carState.Umin << ", Umax=" << carState.Umax << "\n";
        }
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
        switch (packet.getData_ui8(1)) {
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
        if (verboseModeCan) {
          console << ", PrechargeState=" << PRECHARGE_STATE_str[(int)(carState.PrechargeState)] << "\n";
        }

        // Precharge Timer info also available
        break;
      case BMS_BASE_ADDR | 0xF9:
        carState.Tmin = packet.getData_ui16(0) / 10.;
        carState.Tmax = packet.getData_ui16(1) / 10.;
        if (verboseModeCan) {
          console << ", Bat Tmin=" << carState.Tmin << ", Bat Tmax=" << carState.Tmax << "\n";
        }
        break;

      case BMS_BASE_ADDR | 0xFD:

        carState.BatteryErrors.clear();

        if (packet.getData_ui32(0) > 0) { // Saving CPU time in case there are no errors
          for (int i = 0; i < 13; i++) {
            if (packet.getData_b(i)) {
              carState.BatteryErrors.push_front(static_cast<BATTERY_ERROR>(i));
              if (verboseModeCan) {
                console << ", BatErrors=" << carState.batteryErrorsAsString(true) << "\n";
              }
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
        carState.PhotoVoltaicCurrent = carState.Mppt1Current + carState.Mppt2Current + carState.Mppt3Current;

        // MPPT1 Output Voltage V packet.getData_f32(0)
        // MPPT1 Output Current A packet.getData_f32(1)
        if (verboseModeCan) {
          console << ", Mppt1Cur=" << carState.Mppt1Current << "\n";
        }

        break;
      case MPPT2_BASE_ADDR | 0x1:
        carState.Mppt2Current = packet.getData_f32(1);
        carState.PhotoVoltaicCurrent = carState.Mppt1Current + carState.Mppt2Current + carState.Mppt3Current;

        // MPPT2 Output Voltage V packet.getData_f32(0)
        // MPPT2 Output Current A packet.getData_f32(1)
        if (verboseModeCan) {
          console << ", Mppt2Cur=" << carState.Mppt2Current << "\n";
        }

        break;
      case MPPT3_BASE_ADDR | 0x1:
        carState.Mppt3Current = packet.getData_f32(1);
        carState.PhotoVoltaicCurrent = carState.Mppt1Current + carState.Mppt2Current + carState.Mppt3Current;

        // MPPT3 Output Voltage V packet.getData_f32(0)
        // MPPT3 Output Current A packet.getData_f32(1)
        if (verboseModeCan) {
          console << ", Mppt3Cur=" << carState.Mppt3Current << "\n";
        }
        break;
      case MPPT1_BASE_ADDR | 0x2:
        carState.T1 = packet.getData_f32(0);
        if (verboseModeCan) {
          console << "T1=" << carState.T1 << "\n";
        }
      case MPPT2_BASE_ADDR | 0x2:
        carState.T2 = packet.getData_f32(0);
        if (verboseModeCan) {
          console << "T2=" << carState.T2 << "\n";
        }
      case MPPT3_BASE_ADDR | 0x2:
        carState.T3 = packet.getData_f32(0);
        if (verboseModeCan) {
          console << "T3=" << carState.T3 << "\n";
        }
      }
    }
    // vTaskDelay(sleep_polling_ms / portTICK_RATE_MS);
    vTaskDelay(sleep_polling_ms / portTICK_PERIOD_MS);
  }
}

void CANBus::onReceive(int packetSize) {

  CANPacket packet;
  uint64_t rxData = 0;

  xSemaphoreTakeT(mutex);
  packet.setID(CAN.packetId());
  xSemaphoreGive(mutex);

  if (max_ages[packet.getID()] == 0 || (max_ages[packet.getID()] != -1 && millis() - ages[packet.getID()] > max_ages[packet.getID()])) {

    ages[packet.getID()] = millis();
    xSemaphoreTakeT(mutex);
    for (int i = 0; i < packetSize; i++) {
      if (CAN.available()) {
        rxData = rxData | (((uint64_t)CAN.read()) << (i * 8));
      }
    }
    xSemaphoreGive(mutex);

    packet.setData(rxData);

    // Add packet to buffer so task can handle it later
    rxBuffer.push(packet);
  }
}
