#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "service.h"
#include "observation.h"

static const char *TAG = "MAIN";

void app_main(void)
{
    ESP_LOGI(TAG, "=== LoRaWAN Sensor Node - Full Stack Test ===");

    if (!service_init()) {
        ESP_LOGE(TAG, "Service init failed");
        return;
    }

    if (!observation_init()) {
        ESP_LOGE(TAG, "Observation init failed");
        return;
    }

    while (1) {
        // 1. Get clean snapshot from Service Layer
        sensor_snapshot_t snap = service_get_sensor_snapshot();

        // 2. Check thresholds
        service_check_thresholds(&snap);

        // 3. Publish structured observation
        observation_publish(&snap);

        // 4. Power management (placeholder)
        service_update_power_mode();

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}