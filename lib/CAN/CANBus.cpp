//
// CAN Bus
//
#include <Arduino.h>

#include <definitions.h>

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#include <CAN_config.h>
#include <ESP32CAN.h>

#include "CANBus.h"

CAN_device_t CAN_cfg;

SemaphoreHandle_t can_mutex;

void init_can(void) {
  can_mutex = xSemaphoreCreateBinary();

  CAN_cfg.speed = CAN_SPEED; // MPPT & BMS are both running on 125KBPS by default
  CAN_cfg.tx_pin_id = CAN_TX;
  CAN_cfg.rx_pin_id = CAN_RX;
  CAN_cfg.rx_queue = xQueueCreate(CAN_RX_QUEUE, sizeof(CAN_frame_t));
  ESP32Can.CANInit();

  xSemaphoreGive(can_mutex);
}

void read_can_demo_task(void *pvParameter) {
  CAN_frame_t rx_frame;

  while (1) {
    Serial.println("CAN While");
    xSemaphoreTake(can_mutex, portMAX_DELAY);
    Serial.println("Take Mutex");
    if (xQueueReceive(CAN_cfg.rx_queue, &rx_frame, 3 * portTICK_PERIOD_MS) == pdTRUE) {
      if (rx_frame.FIR.B.FF == CAN_frame_std) {
        Serial.println("New standard frame");
      } else {
        Serial.println("New extended Frame");
      }

      Serial.println("from: ");
      Serial.println(rx_frame.MsgID);
      Serial.println("DLC: ");
      Serial.println(rx_frame.FIR.B.DLC);

      for (int i = 0; i < rx_frame.FIR.B.DLC; i++) {
        Serial.println(rx_frame.data.u8[i]);
      }
      Serial.println("--------------------");
    }
    xSemaphoreGive(can_mutex);

    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}