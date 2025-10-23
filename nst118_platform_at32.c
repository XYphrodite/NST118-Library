/**
 * @file nst118_platform_at32.c
 * @brief Platform implementation for AT32F403A
 */

#include "nst118.h"
#include <at32f403a_407.h>
#include "at32f403a_407_gpio.h"
#include "DelayService.h"
#include "i2c_hal.h"

// External I2C context (defined in your main code)
extern i2c_context_t g_nst118_i2c_ctx;

uint8_t platform_i2c_read(uint8_t addr, uint8_t reg, uint8_t* data, size_t len)
{
    g_nst118_i2c_ctx.slaveAddr = addr;
    return i2c_hal_read(&g_nst118_i2c_ctx, reg, data, len) ? 1 : 0;
}

uint8_t platform_i2c_write(uint8_t addr, uint8_t reg, const uint8_t* data, size_t len)
{
    g_nst118_i2c_ctx.slaveAddr = addr;
    return i2c_hal_write(&g_nst118_i2c_ctx, reg, data, len) ? 1 : 0;
}

void platform_delay_us(uint32_t us)
{
    delay_us(us);
}

void platform_pullup_enable(uint8_t state)
{
    gpio_bits_set(GPIOB, GPIO_PINS_4);
}

void platform_pullup_disable(uint8_t state)
{
    gpio_bits_reset(GPIOB, GPIO_PINS_4);
}

