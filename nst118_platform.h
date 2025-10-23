/**
 * @file nst118_platform.h
 * @brief Platform abstraction layer for NST118 driver
 * @details Implement these functions for your specific platform
 * 
 * @copyright MIT License
 */

#ifndef NST118_PLATFORM_H
#define NST118_PLATFORM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

/**
 * @brief Read data from I2C device register
 * 
 * @param dev_addr I2C device address (7-bit)
 * @param reg_addr Register address to read from
 * @param data Buffer to store read data
 * @param len Number of bytes to read
 * @return 1 if successful, 0 otherwise
 */
uint8_t platform_i2c_read(uint8_t dev_addr, uint8_t reg_addr, 
                          uint8_t* data, size_t len);

/**
 * @brief Write data to I2C device register
 * 
 * @param dev_addr I2C device address (7-bit)
 * @param reg_addr Register address to write to
 * @param data Data buffer to write
 * @param len Number of bytes to write
 * @return 1 if successful, 0 otherwise
 */
uint8_t platform_i2c_write(uint8_t dev_addr, uint8_t reg_addr, 
                           const uint8_t* data, size_t len);

/**
 * @brief Delay for specified microseconds
 * 
 * @param us Microseconds to delay
 */
void platform_delay_us(uint32_t us);

/**
 * @brief Set GPIO pin state (optional, for pull-up control)
 * 
 * @param state 1 = high, 0 = low
 */
void platform_pullup_enable(uint8_t state);

/**
 * @brief Clear GPIO pin state (optional, for pull-up control)
 * 
 * @param state 1 = high, 0 = low
 */
void platform_pullup_disable(uint8_t state);

#ifdef __cplusplus
}
#endif

#endif // NST118_PLATFORM_H

