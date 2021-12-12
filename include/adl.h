/**
 * @file adl.h
 * @author Sergey Tkachenko (sergey.iray@yandex.ru)
 * @brief
 * @version 0.1
 * @date 2021-07-03
 *
 * @copyright Copyright (c) 2021
 *
 */
#pragma once

#include "adl_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Config
 *
 */
typedef struct {
    adl_direction_e direction; /** Led panel direction. */
    adl_geometry_e geometry; /** Led array geometry. */
    adl_led_type_e led_type; /** Led type */
    uint16_t width; /** Panel/strip width. */
    uint16_t height; /** Panel height. */
    uint32_t* led_buffer; /** Led buffer used for internal operations. Should be height*width*sizeof(uint32_t). Should not be accessed by user. */
    struct {
        void (*write)(uint8_t* buffer, uint32_t len); /** SPI write method. Provided by user. */
        uint32_t spi_clock_speed; /**SPI clock speed. Critical for WS leds. May be 0 for APA leds */
    } spi_driver;
} adl_config_t;

/**
 * @brief Initializes the library.
 *
 * @param config Config pointer.
 * @return adl_err Error code.
 */
adl_err adl_init(adl_config_t* config);

/**
 * @brief Set led color at position (x, y).
 *
 * @note Coordinates are always in 'display' format regardless of config direction value.
 * Library does row reversal automatically.
 *
 * @note For led strips y position is ignored.
 *
 * @param x Top-left -> top-right direction coordinate.
 * @param y Top->left -> bottom->left direction coordinate.
 * @param color Color.
 * @return adl_err Error code.
 */
adl_err adl_led_set(uint16_t x, uint16_t y, adl_rgba_t color);

/**
 * @brief Sets all leds to one color.
 *
 * @param color Color.
 * @return adl_err Error code.
 */
adl_err adl_fill(adl_rgba_t color);

/**
 * @brief Performs LED color via SPI transmission.
 * Requires user to provide spi_driver in adl_config_t.
 *
 * @return adl_err
 */
adl_err adl_update();

/**
 * @brief Clears all leds.
 * @note Equivalent to settings all led colors to black (0x00000000).
 *
 * @return adl_err Error code.
 */

void adl_hex_to_rgb(const char* hex_color, adl_rgba_t* rgba_color);
void adl_rgb_to_hsv(adl_rgba_t* rgba_color, adl_hsv_t* hsv_color);
void adl_hsv_to_rgb(adl_hsv_t* hsv_color, adl_rgba_t* rgba_color);

adl_err adl_clear();

#ifdef __cplusplus
}
#endif
