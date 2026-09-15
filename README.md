# NST118 Temperature Sensor Library — I2C Driver for STM32 / AT32 / Arduino / ESP32

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![Language: C](https://img.shields.io/badge/language-C-blue.svg)](nst118.h)
[![Platform: MCU](https://img.shields.io/badge/platform-STM32%20%7C%20AT32%20%7C%20Arduino%20%7C%20ESP32-lightgrey.svg)](#compatibility)
[![Context7: indexed](https://img.shields.io/badge/Context7-AT32%20docs-blueviolet.svg)](https://context7.com/xyphrodite)
[![I2C: 0x48/0x49](https://img.shields.io/badge/I2C-0x48%2F0x49-green.svg)](#hardware)

[English](#nst118-temperature-sensor-library--i2c-driver-for-stm32--at32--arduino--esp32) | [Русский](#библиотека-температурного-датчика-nst118)

Minimal, **portable C library for NST118 I2C temperature sensor** — works on any MCU with I2C. Callback-based HAL, no `malloc`, no dependencies. Production-tested in TAFCO LoRaWAN sensor network.

> **Keywords for search:** `nst118`, `temperature sensor`, `i2c temperature sensor`, `embedded temperature driver`, `stm32 temperature sensor`, `at32 temperature sensor`, `arduino temperature sensor`, `esp32 temperature sensor`, `c library`, `hal driver`, `i2c sensor`, `mcu driver`, `12-bit temperature sensor`

## Why this library

NST118 is hard to find a clean C driver for. Most code is vendor-locked or C++ heavy. This is:
- **1 file + 1 header** (`nst118.c` / `nst118.h`) — drop-in
- **Callback I2C** — bring your own `HAL_I2C_Mem_Read` / `Wire.*` / `i2c_read`
- **Deterministic** — static memory only, ~20 ms one-shot conversion
- **Battle-tested** — powers AT32F403A LoRaWAN field devices (see `XYphrodite/TAFCO-Lora-projects`)

## Features

- ✅ **Simple C API** — 3 functions to read temperature
- ✅ **Platform Independent** — callback-based I2C (`nst118_i2c_read_fn` / `write_fn`)
- ✅ **One-shot Mode** — fast ~20 ms measurement, ideal for low-power
- ✅ **Low Power** — shutdown mode (`nst118_set_shutdown`)
- ✅ **High Precision** — 12-bit, 0.0625 °C resolution, -40 to +125 °C
- ✅ **No malloc/free** — static `nst118_t` context only
- ✅ **Dual Sensor** — two addresses on one bus (`0x48` GND / `0x49` VDD)

## 📁 Files

```
nst118.h                  # Library API header — include this
nst118.c                  # Library implementation — add to build
nst118_platform.h         # Platform interface you implement (I2C read/write/delay)
nst118_platform_at32.c    # Reference AT32F403A implementation (HAL I2C)
example.c                 # Minimal usage example
```

## Compatibility

| Platform | I2C API | Status |
|----------|---------|--------|
| **STM32 HAL** | `HAL_I2C_Mem_Read/Write` | ✅ Tested (via AT32 HAL, STM32-compatible) |
| **AT32F403A/407** | `i2c_memory_read` | ✅ Reference impl `nst118_platform_at32.c` |
| **Arduino** | `Wire.requestFrom` / `Wire.write` | ✅ Wrap in callbacks |
| **ESP-IDF / ESP32** | `i2c_master_write_read_device` | ✅ Wrap in callbacks |
| **nRF / Zephyr** | `i2c_write_read` | ✅ Wrap in callbacks |
| **Any MCU** | Any I2C read/write | ✅ Just implement 3 callbacks |

## Quick Start

```c
#include "nst118.h"
#include "nst118_platform.h"  // implement these 3 functions for your platform

// 1. Implement callbacks for your MCU (example for STM32 HAL)
uint8_t my_i2c_read(uint8_t dev_addr, uint8_t reg, uint8_t *data, size_t len) {
    return HAL_I2C_Mem_Read(&hi2c1, dev_addr << 1, reg, 1, data, len, 100) == HAL_OK;
}
uint8_t my_i2c_write(uint8_t dev_addr, uint8_t reg, const uint8_t *data, size_t len) {
    return HAL_I2C_Mem_Write(&hi2c1, dev_addr << 1, reg, 1, (uint8_t*)data, len, 100) == HAL_OK;
}
void my_delay_us(uint32_t us) { delay_us_impl(us); }

// 2. Init
nst118_t sensor;
nst118_init(&sensor, NST118_ADDR_GND, my_i2c_read, my_i2c_write, my_delay_us);

// 3. Read
float temp;
if (nst118_read_temperature(&sensor, &temp)) {
    printf("Temperature: %.2f C\n", temp);
}
```

> Full working example: [`example.c`](example.c) · AT32 wiring: [`nst118_platform_at32.c`](nst118_platform_at32.c)

## API

### Initialization
```c
void nst118_init(nst118_t* sensor, uint8_t i2c_addr,
                 nst118_i2c_read_fn i2c_read,
                 nst118_i2c_write_fn i2c_write,
                 nst118_delay_us_fn delay_us);
```

### Optional Pull-up Control
```c
void nst118_set_pullup(nst118_t* sensor,
                       nst118_gpio_fn pullup_enable,
                       nst118_gpio_fn pullup_disable);
```

### Read Temperature
```c
uint8_t nst118_read_temperature(nst118_t* sensor, float* temperature);
// Returns 1 on success, 0 on failure — takes ~20 ms
```

### Shutdown Mode
```c
uint8_t nst118_set_shutdown(nst118_t* sensor, uint8_t enable);
// 1 = shutdown (low power), 0 = continuous
```

Detailed docs in [`nst118.h`](nst118.h) · [`nst118_platform.h`](nst118_platform.h).

## Hardware

**I2C Addresses:**
- `NST118_ADDR_GND` (`0x48`) — AD pin → GND
- `NST118_ADDR_VDD` (`0x49`) — AD pin → VDD

**Connections:**
- VDD: 2.7 V – 5.5 V
- SDA/SCL: 4.7 kΩ pull-ups required
- AD: GND or VDD selects address

Two sensors can share one bus (one at 0x48, one at 0x49).

## Context7 / LLM Search

This library is **Context7-optimized** for AI code assistants:

- Library ID for Context7: `/xyphrodite/nst118-library`
- `llms.txt` at repo root lists all docs for ingestion
- All code snippets are copy-paste ready with callbacks

If you use Context7, search `NST118` or `xyphrodite/nst118-library`.

## Changelog

See [CHANGELOG.md](CHANGELOG.md).

## Support

- **Issues:** [GitHub Issues](https://github.com/XYphrodite/NST118-Library/issues)
- **Discussions:** [GitHub Discussions](https://github.com/XYphrodite/NST118-Library/discussions)
- **Parent project:** [XYphrodite/TAFCO-Lora-projects](https://github.com/XYphrodite/TAFCO-Lora-projects) (real-world LoRaWAN deployment)

## Authors

- Original implementation: AT32 LoRaWAN Temperature Sensor Project (TAFCO)
- Extraction & maintenance: [Contributors](https://github.com/XYphrodite/NST118-Library/graphs/contributors)

## License

MIT — see [LICENSE](LICENSE).

## Acknowledgments

- NST118 datasheet · AT32 community for field testing

---

# Библиотека температурного датчика NST118

[![Лицензия: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![Язык: C](https://img.shields.io/badge/language-C-blue.svg)](nst118.h)
[![Платформа: MCU](https://img.shields.io/badge/platform-STM32%20%7C%20AT32%20%7C%20Arduino%20%7C%20ESP32-lightgrey.svg)](#совместимость)

Минималистичная **портируемая C-библиотека для датчика NST118 (I2C)** — работает на любом МК. HAL на колбэках, без `malloc`, без зависимостей. Проверена в полевых LoRaWAN-устройствах TAFCO.

> **Ключевые слова:** `nst118`, `датчик температуры`, `i2c датчик температуры`, `драйвер температуры`, `stm32 датчик температуры`, `at32 датчик температуры`, `arduino датчик`, `библиотека c`

## Возможности

- ✅ **Простой C API** — 3 функции для чтения
- ✅ **Платформонезависимая** — I2C через колбэки
- ✅ **One-shot режим** — ~20 мс на измерение, для low-power
- ✅ **Низкое потребление** — режим shutdown
- ✅ **Высокая точность** — 12 бит, 0.0625 °C, -40…+125 °C
- ✅ **Без malloc/free** — только статическая память
- ✅ **Два датчика** — два адреса на одной шине (0x48/0x49)

## 📁 Файлы

```
nst118.h                  # Заголовок API
nst118.c                  # Реализация
nst118_platform.h         # Интерфейс платформы (реализуйте 3 функции)
nst118_platform_at32.c    # Эталон для AT32F403A
example.c                 # Пример
```

## Совместимость

| Платформа | I2C API | Статус |
|-----------|---------|--------|
| **STM32 HAL** | `HAL_I2C_Mem_Read/Write` | ✅ |
| **AT32F403A/407** | `i2c_memory_read` | ✅ Эталон |
| **Arduino** | `Wire.*` | ✅ Через обёртки |
| **ESP-IDF** | `i2c_master_*` | ✅ Через обёртки |
| **Любой МК** | Любой I2C | ✅ Реализуйте 3 колбэка |

## Быстрый старт

```c
#include "nst118.h"
nst118_t sensor;
nst118_init(&sensor, NST118_ADDR_GND, my_i2c_read, my_i2c_write, my_delay_us);
float temp;
if (nst118_read_temperature(&sensor, &temp)) {
    printf("Температура: %.2f C\n", temp);
}
```

Полный пример: [`example.c`](example.c)

## API

```c
void nst118_init(nst118_t* sensor, uint8_t i2c_addr,
                 nst118_i2c_read_fn i2c_read,
                 nst118_i2c_write_fn i2c_write,
                 nst118_delay_us_fn delay_us);
uint8_t nst118_read_temperature(nst118_t* sensor, float* temperature);
uint8_t nst118_set_shutdown(nst118_t* sensor, uint8_t enable);
void nst118_set_pullup(nst118_t* sensor, nst118_gpio_fn en, nst118_gpio_fn dis);
```

## Железо

- `NST118_ADDR_GND` (0x48) — AD → GND
- `NST118_ADDR_VDD` (0x49) — AD → VDD
- VDD 2.7–5.5 В, подтяжки 4.7 кΩ на SDA/SCL

## Changelog

См. [CHANGELOG.md](CHANGELOG.md).

## Поддержка

- **Issues:** [GitHub Issues](https://github.com/XYphrodite/NST118-Library/issues)
- **Родительский проект:** [TAFCO-Lora-projects](https://github.com/XYphrodite/TAFCO-Lora-projects)

## Лицензия

MIT — см. [LICENSE](LICENSE).

<!--
GitHub search keywords: nst118 temperature sensor i2c stm32 at32 arduino esp32 embedded mcu driver c library hal
Context7 libraryId: /xyphrodite/nst118-library
-->
