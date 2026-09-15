# Changelog

All notable changes to the NST118 Library will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.1.0] - 2025-01-23

### Added
- Initial release of NST118 Temperature Sensor Library
- Platform-independent C library with callback-based I2C
- Support for one-shot temperature measurement
- Support for shutdown mode (low power)
- 12-bit resolution (0.0625°C)
- AT32F403A/F407 platform implementation
- Example code for AT32
- Comprehensive README with API documentation
- MIT License

### Features
- Simple C API with minimal dependencies
- No dynamic memory allocation
- Support for dual sensors (0x48 and 0x49 addresses)
- Optional GPIO pull-up control
- Temperature range: -40°C to +125°C
- Conversion time: ~20ms

### Platform Support
- AT32F403A/F407 (with example implementation)
- Easy to port to Arduino, STM32, ESP32, etc.

[1.0.0]: https://github.com/XYphrodite/NST118-Library/releases/tag/v1.0.0

