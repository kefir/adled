#include "adl.h"

#include <stdio.h>
#include <string.h>

/**
 * @brief Checks config sanity. Initializes led buffer.
 * 
 * @param config Pointer to user config.
 * @return adl_err Error code.
 */
static adl_err config_check(adl_config_t* config);

static adl_config_t* cfg = NULL;
static uint32_t led_count = 0;
static uint32_t buffer_size = 0;

adl_err adl_init(adl_config_t* config)
{
    adl_err err = ADL_FAIL;

    led_count = 0;
    buffer_size = 0;

    err = config_check(config);

    if (err == ADL_OK) {
        cfg = config;
    }
    return err;
}

adl_err adl_led_set(uint16_t x, uint16_t y, adl_rgba_t color)
{
    adl_err err = ADL_FAIL;

    if (cfg) {
        if (x < cfg->width && y < cfg->height) {
            uint32_t led_index = 0;

            if (cfg->direction == ADL_DIRECTION_ZIGZAG && cfg->geometry == ADL_GEOMETRY_PANEL) {
                int x_rev = (y % 2) ? cfg->width - x - 1 : x;
                led_index = y * cfg->width + x_rev;
            } else {
                led_index = y * cfg->width + x;
            }
        }
    } else {
        err = ADL_ERR_INIT;
    }
    return err;
}

adl_err adl_fill(adl_rgba_t color)
{
    adl_err err = ADL_FAIL;

    if (cfg) {

    } else {
        err = ADL_ERR_INIT;
    }
    return err;
}

adl_err adl_clear()
{
    adl_err err = ADL_FAIL;

    if (cfg) {

    } else {
        err = ADL_ERR_INIT;
    }
    return err;
}

static adl_err config_check(adl_config_t* config)
{
    adl_err err = ADL_FAIL;

    if (config) {
        if (config->led_buffer) {
            switch (config->geometry) {
            case ADL_GEOMETRY_STRIP:
                config->height = 1;
                err = (config->width == 0) ? ADL_ERR_CONFIG : ADL_OK;
                break;
            case ADL_GEOMETRY_PANEL:
                err = ((config->height == 0) || (config->width == 0)) ? ADL_ERR_CONFIG : ADL_OK;
                break;
            default:
                err = ADL_ERR_CONFIG;
                break;
            }

            if (err == ADL_OK) {
                led_count = config->width * config->height;
                buffer_size = led_count * sizeof(adl_rgba_t);
                memset(config->led_buffer, 0, buffer_size);
            }
        } else {
            err = ADL_ERR_BUFFER;
        }
    }

    return err;
}