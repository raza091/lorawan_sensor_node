#include "observation.h"
#include "esp_log.h"
#include "esp_timer.h"
#include <string.h>

static const char *TAG = "OBSERVATION";

#define DEVICE_ID           0x0001
#define FIRMWARE_VERSION    1

static observation_msg_t last_msg = {0};

bool observation_init(void)
{
    ESP_LOGI(TAG, "Observation layer initialized");
    return true;
}

void observation_publish(const sensor_snapshot_t *snap)
{
    if (snap == NULL) {
        return;
    }

    observation_msg_t msg = {0};

    msg.device_id     = DEVICE_ID;
    msg.timestamp_ms  = snap->timestamp_ms;
    msg.distance_cm   = snap->distance_cm;
    msg.temperature_c = snap->temperature_c;
    msg.battery_v     = snap->battery_v;
    msg.sequence      = snap->sequence;
    msg.firmware_ver  = FIRMWARE_VERSION;

    // Flags
    msg.flags = 0;
    if (snap->tamper_detected) msg.flags |= (1 << 0);
    if (snap->battery_v > 0 && snap->battery_v < 3.3f) msg.flags |= (1 << 1);
    if (!snap->data_valid) msg.flags |= (1 << 2);

    // Store last message
    last_msg = msg;

    // Structured log (this is what you will later send over LoRaWAN)
    ESP_LOGI(TAG, "OBS | Dev:%04lX | Seq:%3d | Dist:%6.1f cm | Temp:%5.1f C | Batt:%4.2f V | Flags:0x%02X | Time:%lu",
             msg.device_id,
             msg.sequence,
             msg.distance_cm,
             msg.temperature_c,
             msg.battery_v,
             msg.flags,
             msg.timestamp_ms);
}

void observation_log_event(const char *event_name, uint32_t value)
{
    ESP_LOGI(TAG, "EVENT | %s = %lu", event_name, value);
}

void observation_print_last(void)
{
    ESP_LOGI(TAG, "Last Observation → Dist: %.1f cm | Seq: %d | Flags: 0x%02X",
             last_msg.distance_cm, last_msg.sequence, last_msg.flags);
}