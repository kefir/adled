#include "adl.h"

#include <stdio.h>
#include <string.h>

static uint8_t DUMMY_BYTE = 0x00;

/**
 * @brief Checks config sanity. Initializes led buffer.
 *
 * @param config Pointer to user config.
 * @return adl_err Error code.
 */
static adl_err config_check(adl_config_t* config);

static float max(float a, float b, float c);
static float min(float a, float b, float c);

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

adl_err adl_led_set(uint16_t x, uint16_t y, adl_rgb_t color)
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
            cfg->led_buffer[led_index] = (uint32_t)color.color;
            err = ADL_OK;
        }
    } else {
        err = ADL_ERR_INIT;
    }
    return err;
}

adl_err adl_fill(adl_rgb_t color)
{
    adl_err err = ADL_FAIL;

    if (cfg) {
        for (uint32_t i = 0; i < led_count; i++) {
            cfg->led_buffer[i] = (uint32_t)color.color;
        }
        err = ADL_OK;
    } else {
        err = ADL_ERR_INIT;
    }
    return err;
}

adl_err adl_update()
{
    adl_err err = ADL_FAIL;
    if (cfg->spi_driver.write) {

        switch (cfg->led_type) {
        case ADL_LED_TYPE_APA102:
            break;
        case ADL_LED_TYPE_WS2812:
            cfg->spi_driver.write(&DUMMY_BYTE, sizeof(uint8_t));
            cfg->spi_driver.write((uint8_t*)cfg->led_buffer, led_count * sizeof(adl_rgb_t));
            cfg->spi_driver.write(&DUMMY_BYTE, sizeof(uint8_t));
            break;
        default:
            break;
        }
    }
    return err;
}

adl_err adl_clear()
{
    adl_err err = ADL_FAIL;

    if (cfg) {
        memset(cfg->led_buffer, 0, cfg->height * cfg->width * sizeof(adl_rgba_t));
        err = ADL_OK;
    } else {
        err = ADL_ERR_INIT;
    }
    return err;
}

void adl_hex_to_rgb(const char* hex_color, adl_rgb_t* rgb_color)
{
    if (hex_color && rgb_color) {
        if (hex_color[0] == '#') {
            // sscanf(hex_color, "#%02x%02x%02x", (unsigned int*)&rgb_color->r, (unsigned int*)&rgb_color->g, (unsigned int*)&rgb_color->b);
        } else {
            // sscanf(hex_color, "%02x%02x%02x", (unsigned int*)&rgb_color->r, (unsigned int*)&rgb_color->g, (unsigned int*)&rgb_color->b);
        }
    }
}

void adl_rgb_to_hsv(adl_rgb_t* rgb_color, adl_hsv_t* hsv_color)
{
    if (hsv_color && rgb_color) {
        float rd = (float)rgb_color->r / 255.;
        float gd = (float)rgb_color->g / 255.;
        float bd = (float)rgb_color->b / 255.;
        float max_f = max(rd, gd, bd);
        float min_f = min(rd, gd, bd);
        float h = 0.;
        float s = 0.;
        float v = max_f;

        double d = max_f - min_f;
        s = max_f == 0 ? 0 : d / max_f;

        if (max_f == min_f) {
            h = 0.f; // achromatic
        } else {
            if (max_f == rd) {
                h = (gd - bd) / d + (gd < bd ? 6.f : 0.f);
            } else if (max_f == gd) {
                h = (bd - rd) / d + 2.f;
            } else if (max_f == bd) {
                h = (rd - gd) / d + 4.f;
            }
            h /= 6.f;
        }

        hsv_color->h = h;
        hsv_color->s = s;
        hsv_color->v = v;
    }
}

void adl_hsv_to_rgb(adl_hsv_t* hsv_color, adl_rgba_t* rgb_color)
{
    if (hsv_color && rgb_color) {
        float r = 0.f;
        float g = 0.f;
        float b = 0.f;
        int i = (int)(hsv_color->h * 6.f);
        float f = hsv_color->h * 6.f - i;
        float p = hsv_color->v * (1.f - hsv_color->s);
        float q = hsv_color->v * (1.f - f * hsv_color->s);
        float t = hsv_color->v * (1.f - (1.f - f) * hsv_color->s);

        switch (i % 6) {
        case 0:
            r = hsv_color->v;
            g = t;
            b = p;
            break;
        case 1:
            r = q;
            g = hsv_color->v;
            b = p;
            break;
        case 2:
            r = p;
            g = hsv_color->v;
            b = t;
            break;
        case 3:
            r = p;
            g = q;
            b = hsv_color->v;
            break;
        case 4:
            r = t;
            g = p;
            b = hsv_color->v;
            break;
        case 5:
            r = hsv_color->v;
            g = p;
            b = q;
            break;
        }

        rgb_color->r = r * 255;
        rgb_color->g = g * 255;
        rgb_color->b = b * 255;
    }
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

static float max(float a, float b, float c)
{
    return ((a > b) ? (a > c ? a : c) : (b > c ? b : c));
}
static float min(float a, float b, float c)
{
    return ((a < b) ? (a < c ? a : c) : (b < c ? b : c));
}