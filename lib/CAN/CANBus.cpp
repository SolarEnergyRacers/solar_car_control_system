//
// CAN Bus
//
#include <Arduino.h>

#include <definitions.h>

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#include <CAN_config.h>
#include <ESP32CAN.h>

#include <CANBus.h>

extern CAN_device_t CAN_cfg;
extern CanBus can;

void CanBus::re_init() { CanBus::init(); }

void CanBus::init() {

  mutex = xSemaphoreCreateMutex();

  CAN_cfg.speed = CAN_SPEED; // MPPT & BMS are both running on 125KBPS by default
  CAN_cfg.tx_pin_id = CAN_TX;
  CAN_cfg.rx_pin_id = CAN_RX;
  CAN_cfg.rx_queue = xQueueCreate(CAN_RX_QUEUE, sizeof(CAN_frame_t));
  ESP32Can.CANInit();

  xSemaphoreGive(mutex);
}

void read_can_demo_task(void *pvParameter) {

  can.init();

  CAN_frame_t rx_frame;

  while (1) {
    debug_printf_l3("CAN While%s", "\n");
    xSemaphoreTake(can.mutex, portMAX_DELAY);

    debug_printf_l3("Take Mutex%s", "\n");
    if (xQueueReceive(CAN_cfg.rx_queue, &rx_frame, 3 * portTICK_PERIOD_MS) == pdTRUE) {
      if (rx_frame.FIR.B.FF == CAN_frame_std) {
        Serial.println("New standard frame");
      } else {
        Serial.println("New extended Frame");
      }

      debug_printf_l3("from: %d\tDLC: %d\n", rx_frame.MsgID, rx_frame.FIR.B.DLC);

      for (int i = 0; i < rx_frame.FIR.B.DLC; i++) {
        debug_printf_l3(" %02d", rx_frame.data.u8[i]);
      }
      debug_printf_l3("\n--------------------%s", "\n");
    }
    xSemaphoreGive(can.mutex);

    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}