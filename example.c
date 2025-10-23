/**
 * @file example.c
 * @brief NST118 usage example
 */

#include "nst118.h"
#include <stdio.h>

// Platform-specific functions (implement for your platform)
extern uint8_t platform_i2c_read(uint8_t addr, uint8_t reg, uint8_t* data, size_t len);
extern uint8_t platform_i2c_write(uint8_t addr, uint8_t reg, const uint8_t* data, size_t len);
extern void platform_delay_us(uint32_t us);
extern void platform_pullup_enable(uint8_t state);
extern void platform_pullup_disable(uint8_t state);

int main(void)
{
    nst118_t sensor;
    float temperature;
    
    // Initialize sensor with I2C address 0x48 (AD pin to GND)
    nst118_init(&sensor, NST118_ADDR_GND, 
                platform_i2c_read, 
                platform_i2c_write, 
                platform_delay_us);
    
    // Optional: Set pull-up control
    nst118_set_pullup(&sensor, platform_pullup_enable, platform_pullup_disable);
    
    // Enable shutdown mode for low power
    nst118_set_shutdown(&sensor, 1);
    
    // Main loop
    while (1) {
        // Read temperature (triggers one-shot conversion)
        if (nst118_read_temperature(&sensor, &temperature)) {
            printf("Temperature: %.2f°C\n", temperature);
        } else {
            printf("Error reading temperature\n");
        }
        
        // Wait 1 second
        platform_delay_us(1000000);
    }
    
    return 0;
}

