/**
 * @file nst118.c
 * @brief NST118 Temperature Sensor Driver Implementation
 * @license MIT
 */

#include "nst118.h"

// Initialize sensor
void nst118_init(nst118_t* sensor, uint8_t i2c_addr,
                 nst118_i2c_read_fn i2c_read,
                 nst118_i2c_write_fn i2c_write,
                 nst118_delay_us_fn delay_us)
{
    sensor->i2c_addr = i2c_addr;
    sensor->i2c_read = i2c_read;
    sensor->i2c_write = i2c_write;
    sensor->delay_us = delay_us;
    sensor->pullup_enable = 0;
    sensor->pullup_disable = 0;
    
    // Set default configuration (shutdown mode, 12-bit)
    uint8_t config[2] = {0xA0, 0x60};  // 0xA060 in big-endian
    i2c_write(i2c_addr, NST118_REG_CONFIG, config, 2);
}

// Set pull-up control functions
void nst118_set_pullup(nst118_t* sensor, nst118_gpio_fn pullup_enable, nst118_gpio_fn pullup_disable)
{
    sensor->pullup_enable = pullup_enable;
    sensor->pullup_disable = pullup_disable;
}

// Read configuration register
static uint8_t read_config(nst118_t* sensor, uint16_t* config)
{
    uint8_t data[2];
    
    if (!sensor->i2c_read(sensor->i2c_addr, NST118_REG_CONFIG, data, 2)) {
        return 0;
    }
    
    // Convert from big-endian
    *config = ((uint16_t)data[0] << 8) | data[1];
    return 1;
}

// Write configuration register
static uint8_t write_config(nst118_t* sensor, uint16_t config)
{
    uint8_t data[2];
    
    // Convert to big-endian
    data[0] = (config >> 8) & 0xFF;
    data[1] = config & 0xFF;
    
    return sensor->i2c_write(sensor->i2c_addr, NST118_REG_CONFIG, data, 2);
}

// Read temperature from sensor
uint8_t nst118_read_temperature(nst118_t* sensor, float* temperature)
{
    uint16_t config;
    uint8_t cnt = 0;
    uint8_t temp_data[2];
    
    // Enable pull-up if configured
    if (sensor->pullup_enable) {
        sensor->pullup_enable(1);
    }
    
    // Read configuration
    if (!read_config(sensor, &config)) {
        if (sensor->pullup_disable) {
            sensor->pullup_disable(0);
        }
        return 0;
    }
    
    // Trigger one-shot conversion
    config |= NST118_CONFIG_OS;
    if (!write_config(sensor, config)) {
        if (sensor->pullup_disable) {
            sensor->pullup_disable(0);
        }
        return 0;
    }
    
    // Wait for conversion to complete (OS bit goes high)
    sensor->delay_us(200);
    
    while (cnt < 5) {
        sensor->delay_us(100);
        if (!read_config(sensor, &config)) {
            if (sensor->pullup_disable) {
                sensor->pullup_disable(0);
            }
            return 0;
        }
        if (config & NST118_CONFIG_OS) {
            break;  // Conversion complete
        }
        cnt++;
    }
    
    if (cnt == 5) {
        // Timeout
        if (sensor->pullup_disable) {
            sensor->pullup_disable(0);
        }
        return 0;
    }
    
    sensor->delay_us(100);
    
    // Read temperature register
    if (!sensor->i2c_read(sensor->i2c_addr, NST118_REG_TEMPERATURE, temp_data, 2)) {
        if (sensor->pullup_disable) {
            sensor->pullup_disable(0);
        }
        return 0;
    }
    
    // Convert to temperature (12-bit mode: bits [15:4])
    // Temperature = integer part + (fractional part * 0.0625)
    *temperature = (int8_t)temp_data[0] + ((temp_data[1] >> 4) * 0.0625f);
    
    // Disable pull-up if configured
    if (sensor->pullup_disable) {
        sensor->pullup_disable(0);
    }
    
    return 1;
}

// Set shutdown mode
uint8_t nst118_set_shutdown(nst118_t* sensor, uint8_t enable)
{
    uint16_t config;
    
    if (!read_config(sensor, &config)) {
        return 0;
    }
    
    if (enable) {
        config |= NST118_CONFIG_SD;   // Set shutdown bit
    } else {
        config &= ~NST118_CONFIG_SD;  // Clear shutdown bit
    }
    
    return write_config(sensor, config);
}

