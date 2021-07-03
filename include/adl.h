#pragma once

#include <stddef.h>
#include <stdint.h>

/**
 * @brief Error codes
 * 
 */
typedef enum {
    ADL_OK, /**< No error */
    ADL_ERR_INIT, /**< Initialization error */
    ADL_ERR_CONFIG, /**< Configuration error */
    ADL_ERR_BUFFER, /**< Null buffer pointer */
    ADL_FAIL /**< Unspecified error */
} adl_err;

/**
 * @brief Led array geometry
 * 
 */
typedef enum {
    ADL_GEOMETRY_STRIP, /**< Leds are in one continous line */
    ADL_GEOMETRY_RECTANGLE /**< Leds are configured as a rectangular panel */
} adl_geometry_e;

/**
 * @brief Led panel direction
 * @note Irrelevant for ADL_GEOMETRY_STRIP
 */
typedef enum {
    ADL_DIRECTION_STRAIGHT, /**< Even rows have same direction as odd ones */
    ADL_DIRECTION_ZIGZAG /**< Even rows are reversed  */
} adl_direction_e;

typedef struct {
    adl_direction_e direction;
    adl_geometry_e geometry;
    uint16_t width;
    uint16_t height;
    void* led_buffer;
} adl_config_t;

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} adl_rgb_t;

typedef struct {
    float h;
    float s;
    float v;
} adl_hsv_t;

adl_err adl_init(adl_config_t* config);

adl_err adl_led_set(uint16_t led_id, adl_rgb_t color);
adl_err adl_fill(adl_rgb_t color);
adl_err adl_clear();

// void adl_hex_to_rgb(const char* hexColor, adl_rgb_t* color);
// void adl_rgb_to_hsv(adl_rgb_t* rgbColor, adl_hsv_t* hsvColor);
// void adl_hsv_to_rgb(adl_hsv_t* hsvColor, adl_rgb_t* rgbColor);