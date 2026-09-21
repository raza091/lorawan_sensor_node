#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "service.h"   // for sensor_snapshot_t

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint32_t device_id;
    uint32_t timestamp_ms;
    float    distance_cm;
    float    temperature_c;
    float    battery_v;
    uint8_t  flags;          // bit0: tamper, bit1: low battery, bit2: invalid data
    uint8_t  sequence;
    uint8_t  firmware_ver;
} observation_msg_t;

/**
 * @brief Initialize observation layer
 */
bool observation_init(void);

/**
 * @brief Publish a structured observation message (from snapshot)
 */
void observation_publish(const sensor_snapshot_t *snap);

/**
 * @brief Log a simple event
 */
void observation_log_event(const char *event_name, uint32_t value);

/**
 * @brief Print the last published message (for debugging)
 */
void observation_print_last(void);

#ifdef __cplusplus
}
#endif