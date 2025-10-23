/**
 * @file nst118.h
 * @brief NST118 Temperature Sensor Driver
 * @details Simple I2C temperature sensor driver for NST118
 * 
 * Features:
 * - I2C communication (addresses 0x48 and 0x49)
 * - One-shot temperature measurement mode
 * - Shutdown mode for low power
 * - 12-bit resolution (0.0625°C)
 * - Temperature range: -40°C to +125°C
 * 
 * @author Based on AT32 LoRaWAN Temperature Sensor project
 * @version 1.0.0
 * @license MIT
 */

#ifndef NST118_H
#define NST118_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// I2C addresses
#define NST118_ADDR_GND     0x48  // AD pin to GND
#define NST118_ADDR_VDD     0x49  // AD pin to VDD

// Register addresses
#define NST118_REG_TEMPERATURE  0x00
#define NST118_REG_CONFIG       0x01
#define NST118_REG_T_LOW        0x02
#define NST118_REG_T_HIGH       0x03

// Configuration bits
#define NST118_CONFIG_OS        (1 << 15)  // One-shot trigger
#define NST118_CONFIG_SD        (1 << 8)   // Shutdown mode

/**
 * @brief I2C read function type
 * @param dev_addr I2C device address (7-bit)
 * @param reg_addr Register address
 * @param data Buffer to store data
 * @param len Number of bytes to read
 * @return true if successful, false otherwise
 */
typedef uint8_t (*nst118_i2c_read_fn)(uint8_t dev_addr, uint8_t reg_addr, uint8_t* data, size_t len);

/**
 * @brief I2C write function type
 * @param dev_addr I2C device address (7-bit)
 * @param reg_addr Register address
 * @param data Data to write
 * @param len Number of bytes to write
 * @return true if successful, false otherwise
 */
typedef uint8_t (*nst118_i2c_write_fn)(uint8_t dev_addr, uint8_t reg_addr, const uint8_t* data, size_t len);

/**
 * @brief Delay function type
 * @param us Microseconds to delay
 */
typedef void (*nst118_delay_us_fn)(uint32_t us);

/**
 * @brief GPIO control function type (optional)
 * @param state true = high, false = low
 */
typedef void (*nst118_gpio_fn)(uint8_t state);

/**
 * @brief NST118 sensor context
 */
typedef struct {
    uint8_t i2c_addr;                   // I2C address
    nst118_i2c_read_fn i2c_read;        // I2C read function
    nst118_i2c_write_fn i2c_write;      // I2C write function
    nst118_delay_us_fn delay_us;        // Delay function
    nst118_gpio_fn pullup_enable;       // Pull-up enable (optional)
    nst118_gpio_fn pullup_disable;      // Pull-up disable (optional)
} nst118_t;

/**
 * @brief Initialize NST118 sensor
 * @param sensor Pointer to sensor context
 * @param i2c_addr I2C address (NST118_ADDR_GND or NST118_ADDR_VDD)
 * @param i2c_read I2C read function
 * @param i2c_write I2C write function
 * @param delay_us Delay function
 */
void nst118_init(nst118_t* sensor, uint8_t i2c_addr,
                 nst118_i2c_read_fn i2c_read,
                 nst118_i2c_write_fn i2c_write,
                 nst118_delay_us_fn delay_us);

/**
 * @brief Set pull-up control functions (optional)
 * @param sensor Pointer to sensor context
 * @param pullup_enable Function to enable pull-up
 * @param pullup_disable Function to disable pull-up
 */
void nst118_set_pullup(nst118_t* sensor, nst118_gpio_fn pullup_enable, nst118_gpio_fn pullup_disable);

/**
 * @brief Read temperature from sensor
 * @param sensor Pointer to sensor context
 * @param temperature Pointer to store temperature (°C)
 * @return 1 if successful, 0 if failed
 * 
 * @note Automatically triggers one-shot conversion in shutdown mode
 * @note Takes approximately 20ms for conversion
 */
uint8_t nst118_read_temperature(nst118_t* sensor, float* temperature);

/**
 * @brief Set shutdown mode
 * @param sensor Pointer to sensor context
 * @param enable 1 = shutdown mode, 0 = continuous mode
 * @return 1 if successful, 0 if failed
 */
uint8_t nst118_set_shutdown(nst118_t* sensor, uint8_t enable);

#ifdef __cplusplus
}
#endif

#endif // NST118_H

