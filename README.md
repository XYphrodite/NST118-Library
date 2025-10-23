# NST118 Temperature Sensor Library

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Platform](https://img.shields.io/badge/platform-MCU-blue.svg)](https://github.com/yourusername/NST118-Library)

Minimal, portable C library for NST118 I2C temperature sensor.

## Features

- ✅ **Simple C API** - Easy to integrate
- ✅ **Platform Independent** - Callback-based I2C
- ✅ **One-shot Mode** - Fast measurements (~20ms)
- ✅ **Low Power** - Shutdown mode support
- ✅ **High Precision** - 12-bit resolution (0.0625°C)
- ✅ **No malloc/free** - Static memory only
- ✅ **Dual Sensor** - Support for two sensors on one bus

## 📁 Files

```
nst118.h                    # Library API header
nst118.c                    # Library implementation
nst118_platform.h           # Platform interface (implement these functions)
nst118_platform_at32.c      # AT32 reference implementation
example.c                   # Usage example
```

## Quick Start

```c
#include "nst118.h"
#include "nst118_platform.h"  // Implement these functions for your platform

// See nst118_platform.h for function declarations
// Example implementations in nst118_platform_at32.c

// 2. Create and initialize sensor
nst118_t sensor;
nst118_init(&sensor, NST118_ADDR_GND, my_i2c_read, my_i2c_write, my_delay_us);

// 3. Read temperature
float temp;
if (nst118_read_temperature(&sensor, &temp)) {
    printf("Temperature: %.2f°C\n", temp);
}
```

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
// Returns 1 on success, 0 on failure
// Takes ~20ms for conversion
```

### Shutdown Mode
```c
uint8_t nst118_set_shutdown(nst118_t* sensor, uint8_t enable);
// 1 = shutdown (low power), 0 = continuous
```

## Hardware

**I2C Addresses:**
- `NST118_ADDR_GND` (0x48) - AD pin to GND
- `NST118_ADDR_VDD` (0x49) - AD pin to VDD

**Connections:**
- VDD: 2.7V - 5.5V
- SDA/SCL: Require 4.7kΩ pull-up resistors
- AD: Connect to GND or VDD for address selection

## Changelog

See [CHANGELOG.md](CHANGELOG.md) for version history.

## Support

- **Issues**: [GitHub Issues](https://github.com/yourusername/NST118-Library/issues)
- **Discussions**: [GitHub Discussions](https://github.com/yourusername/NST118-Library/discussions)

## Authors

- Original implementation: AT32 LoRaWAN Temperature Sensor Project
- Library extraction and maintenance: [Contributors](https://github.com/yourusername/NST118-Library/contributors)

## License

MIT License - see [LICENSE](LICENSE) file for details

## Acknowledgments

- NST118 sensor datasheet and specifications
- AT32 community for testing and feedback

