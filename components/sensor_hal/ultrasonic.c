#include "ultrasonic.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "esp_log.h"
#include "esp_rom_sys.h"          // for esp_rom_delay_us
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "ULTRASONIC";

// Change these according to your hardware
#define TRIG_PIN    GPIO_NUM_5
#define ECHO_PIN    GPIO_NUM_18

#define TIMEOUT_US  30000         // 30 ms timeout

bool ultrasonic_init(void)
{
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << TRIG_PIN),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    ESP_ERROR_CHECK(gpio_config(&io_conf));

    io_conf.pin_bit_mask = (1ULL << ECHO_PIN);
    io_conf.mode = GPIO_MODE_INPUT;
    ESP_ERROR_CHECK(gpio_config(&io_conf));

    // Make sure TRIG starts low
    gpio_set_level(TRIG_PIN, 0);

    ESP_LOGI(TAG, "Ultrasonic sensor initialized (TRIG=%d, ECHO=%d)", TRIG_PIN, ECHO_PIN);
    return true;
}

float ultrasonic_get_distance_cm(void)
{
    // Trigger pulse
    gpio_set_level(TRIG_PIN, 0);
    esp_rom_delay_us(2);
    gpio_set_level(TRIG_PIN, 1);
    esp_rom_delay_us(10);
    gpio_set_level(TRIG_PIN, 0);

    // Wait for echo to go high
    int64_t start = esp_timer_get_time();
    while (gpio_get_level(ECHO_PIN) == 0) {
        if (esp_timer_get_time() - start > TIMEOUT_US) {
            ESP_LOGW(TAG, "Echo start timeout");
            return -1.0f;
        }
    }

    // Measure pulse width
    start = esp_timer_get_time();
    while (gpio_get_level(ECHO_PIN) == 1) {
        if (esp_timer_get_time() - start > TIMEOUT_US) {
            ESP_LOGW(TAG, "Echo end timeout");
            return -1.0f;
        }
    }
    int64_t end = esp_timer_get_time();

    // Distance calculation (speed of sound ≈ 343 m/s → 0.0343 cm/µs)
    float distance = (float)(end - start) * 0.0343f / 2.0f;

    // Basic sanity check
    if (distance < 2.0f || distance > 400.0f) {
        return -1.0f;
    }

    return distance;
}

bool ultrasonic_is_ready(void)
{
    return true;   // You can later add a real health check
}