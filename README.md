# LoRaWAN Sensor Node Firmware (ESP-IDF)

Layered ESP32-S3 firmware skeleton for an industrial sensor node. The sensor, service and observation layers are in place and build in CI. The LoRaWAN radio uplink is the next layer and is not implemented yet.

![Platform](https://img.shields.io/badge/Platform-ESP32--S3-red)
![Framework](https://img.shields.io/badge/Framework-ESP--IDF-blue)
![CI](https://github.com/raza091/lorawan_sensor_node/actions/workflows/ci.yml/badge.svg)
![Status](https://img.shields.io/badge/Status-in%20development-yellow)

## Architecture

```
main/lorawan_sensor_node.c      1 s loop: snapshot ─► thresholds ─► publish ─► power mode
components/
├── sensor_hal/ultrasonic.c     TRIG/ECHO ultrasonic driver, TRIG GPIO5 / ECHO GPIO18, 30 ms timeout
├── service/service.c           sensor snapshot (timestamp, sequence, validity) and threshold checks
└── observation/observation.c   structured message: device ID, firmware version, flags, sequence
```

| Layer | Responsibility |
|---|---|
| Sensor HAL | Talks to hardware only. Returns a distance or an error. |
| Service | Turns raw readings into a validated snapshot and applies alert thresholds. |
| Observation | Builds the fixed-format message that will become the LoRaWAN payload. Currently logged over UART. |

Observation flags: bit 0 tamper, bit 1 low battery (below 3.3 V), bit 2 invalid reading.

## Status

- [x] Ultrasonic distance driver
- [x] Snapshot and threshold service layer
- [x] Structured observation message
- [x] GitHub Actions build for ESP32-S3
- [ ] Temperature, battery and tamper inputs (placeholders today)
- [ ] LoRaWAN uplink and payload encoding
- [ ] Sleep and power management

## Build

```bash
idf.py set-target esp32s3
idf.py build flash monitor
```

## Author

Ali Raza Tariq. [LinkedIn](https://www.linkedin.com/in/ali-raza-embedded-fpga) · [GitHub](https://github.com/raza091)
