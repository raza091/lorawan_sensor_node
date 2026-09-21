#pragma once

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the ultrasonic sensor
 * @return true on success, false on failure
 */
bool ultrasonic_init(void);

/**
 * @brief Get distance in centimeters
 * @return Distance in cm, or -1.0f on error
 */
float ultrasonic_get_distance_cm(void);

/**
 * @brief Check if sensor is ready
 */
bool ultrasonic_is_ready(void);

#ifdef __cplusplus
}
#endif