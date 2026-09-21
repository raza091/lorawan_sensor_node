#include "service.h"
#include "ultrasonic.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "SERVICE";

static sensor_snapshot_t latest_snapshot = {0};
static uint8_t sequence_counter = 0;

bool service_init(void)
{
    if (!ultrasonic_init()) {
        ESP_LOGE(TAG, "Failed to initialize ultrasonic sensor");
        return false;
    }

    ESP_LOGI(TAG, "Service layer initialized");
    return true;
}

sensor_snapshot_t service_get_sensor_snapshot(void)
{
    sensor_snapshot_t snap = {0};

    float distance = ultrasonic_get_distance_cm();

    snap.timestamp_ms = (uint32_t)(esp_timer_get_time() / 1000);
    snap.sequence = sequence_counter++;

    if (distance < 0.0f) {
        snap.data_valid = false;
        snap.distance_cm = -1.0f;
    } else {
        snap.data_valid = true;
        snap.distance_cm = distance;
    }

    // Placeholders for other sensors (will be filled later)
    snap.temperature_c = 0.0f;
    snap.battery_v = 0.0f;
    snap.tamper_detected = false;

    // Store the latest snapshot
    latest_snapshot = snap;

    return snap;
}

void service_check_thresholds(const sensor_snapshot_t *snap)
{
    if (!snap->data_valid) {
        ESP_LOGW(TAG, "Invalid data - skipping threshold check");
        return;
    }

    // Example threshold logic
    if (snap->distance_cm < 10.0f) {
        ESP_LOGW(TAG, "ALERT: Object very close (%.1f cm)", snap->distance_cm);
    } else if (snap->distance_cm > 300.0f) {
        ESP_LOGI(TAG, "Distance is far (%.1f cm)", snap->distance_cm);
    }
}

void service_update_power_mode(void)
{
    // Placeholder for future power management
    // Example: decide between active / light-sleep / deep-sleep
}