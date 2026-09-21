#pragma once

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    float    distance_cm;       // Ultrasonic
    float    temperature_c;     // Placeholder for now
    float    battery_v;         // Placeholder for now
    bool     tamper_detected;   // Placeholder
    bool     data_valid;
    uint32_t timestamp_ms;
    uint8_t  sequence;
} sensor_snapshot_t;

/**
 * @brief Initialize the service layer
 */
bool service_init(void);

/**
 * @brief Get the latest validated sensor snapshot
 */
sensor_snapshot_t service_get_sensor_snapshot(void);

/**
 * @brief Check thresholds and update internal state
 */
void service_check_thresholds(const sensor_snapshot_t *snap);

/**
 * @brief Update power management decisions
 */
void service_update_power_mode(void);

#ifdef __cplusplus
}
#endif