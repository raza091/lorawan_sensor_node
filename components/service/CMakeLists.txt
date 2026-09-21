idf_component_register(
    SRCS "service.c"
    INCLUDE_DIRS "include"
    REQUIRES sensor_hal esp_timer
)