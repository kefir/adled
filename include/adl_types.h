#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ADL_RGB_CHANNELS 3

/**
 * @brief Error codes.
 *
 */
typedef enum {
    ADL_OK, /** No error. */
    ADL_ERR_INIT, /** Initialization error. */
    ADL_ERR_CONFIG, /** Configuration error. */
    ADL_ERR_BUFFER, /** Null buffer pointer. */
    ADL_FAIL /** Unspecified error. */
} adl_err;

typedef enum {
    ADL_LED_TYPE_APA102 = 0U,
    ADL_LED_TYPE_WS2812
} adl_led_type_e;

/**
 * @brief Led array geometry.
 *
 */
typedef enum {
    ADL_GEOMETRY_STRIP, /** Leds are in one continous line */
    ADL_GEOMETRY_PANEL /** Leds are configured as a rectangular panel */
} adl_geometry_e;

/**
 * @brief Led panel direction.
 * @note Irrelevant for ADL_GEOMETRY_STRIP.
 */
typedef enum {
    ADL_DIRECTION_STRAIGHT, /** Even rows have same direction as odd ones */
    ADL_DIRECTION_ZIGZAG /** Even rows are reversed  */
} adl_direction_e;

/**
 * @brief RGBA color definition.
 * @note Arranged as RGBA with 8 bits per channel.
 */
typedef union {
    struct {
        uint32_t r : 8; /** Red channel. */
        uint32_t g : 8; /** Green channel. */
        uint32_t b : 8; /** Blue channel. */
        uint32_t a : 8; /** Alpha channel. */
    };
    uint32_t color; /** Direct access color. */
} adl_rgba_t;

/**
 * @brief RGB color definition. Library default.
 * @note Arranged as RGB with 8 bits per channel (RGB888).
 */
typedef union {
    struct {
        uint32_t r : 8; /** Red channel. */
        uint32_t g : 8; /** Green channel. */
        uint32_t b : 8; /** Blue channel. */
    };
    uint8_t color[ADL_RGB_CHANNELS]; /** Direct access color. */
} adl_rgb_t;

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
    float h; /** Hue. */
    float s; /** Saturation. */
    float v; /** Value/brightness */
} adl_hsv_t;

#ifdef __cplusplus
}
#endif
