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

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

/**
 * @brief Error codes.
 * 
 */
typedef enum {
    ADL_OK, /**< No error. */
    ADL_ERR_INIT, /**< Initialization error. */
    ADL_ERR_CONFIG, /**< Configuration error. */
    ADL_ERR_BUFFER, /**< Null buffer pointer. */
    ADL_FAIL /**< Unspecified error. */
} adl_err;

/**
 * @brief Led array geometry.
 * 
 */
typedef enum {
    ADL_GEOMETRY_STRIP, /**< Leds are in one continous line */
    ADL_GEOMETRY_PANEL /**< Leds are configured as a rectangular panel */
} adl_geometry_e;

/**
 * @brief Led panel direction.
 * @note Irrelevant for ADL_GEOMETRY_STRIP.
 */
typedef enum {
    ADL_DIRECTION_STRAIGHT, /**< Even rows have same direction as odd ones */
    ADL_DIRECTION_ZIGZAG /**< Even rows are reversed  */
} adl_direction_e;

/**
 * @brief Config 
 * 
 */
typedef struct {
    adl_direction_e direction; /**< Led panel direction. */
    adl_geometry_e geometry; /**< Led array geometry. */
    uint16_t width; /**< Panel/strip width. */
    uint16_t height; /**< Panel height. */
    uint32_t* led_buffer; /**< Led buffer used for internal operations. Should be height*width*sizeof(uint32_t). Should not be accessed by user. */
} adl_config_t;

/**
 * @brief RGBA color definition. Library default.
 * @note Arranged as RGBA with 8 bits per channel.
 */
typedef union {
    struct {
        uint32_t r : 8; /**< Red channel. */
        uint32_t g : 8; /**< Green channel. */
        uint32_t b : 8; /**< Blue channel. */
        uint32_t a : 8; /**< Alpha channel. */
    };
    uint32_t color; /**< Direct access color. */
} adl_rgba_t;

/**
 * @brief RGB565 color definition.
 * @note Arranged as RGB.
 */
typedef union {
    struct {
        uint16_t r : 5;
        uint16_t g : 6;
        uint16_t b : 5;
    };
    uint16_t color;
} adl_rgb565_t;

/**
 * @brief HSV/HSB color definition.
 * 
 */
typedef struct {
    float h; /**< Hue. */
    float s; /**< Saturation. */
    float v; /**< Value/brightness */
} adl_hsv_t;

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
 * @brief Clears all leds.
 * @note Equivalent to settings all led colors to black (0x00000000).
 * 
 * @return adl_err Error code.
 */
adl_err adl_clear();

#ifdef __cplusplus
}
#endif
