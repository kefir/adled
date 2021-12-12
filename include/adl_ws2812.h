#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "adl_types.h"

#define WS28_ZERO_L 0xC
#define WS28_ONE_L 0xE
#define WS28_ZERO_H 0xC0
#define WS28_ONE_H 0xE0

#define WS28_BUFF_MULT 4
#define WS28_LED_SIZE 3
#define WS28_LED_BUFF_SIZE (WS28_LED_SIZE * WS28_BUFF_MULT)

typedef union {
    struct {
        uint8_t g;
        uint8_t r;
        uint8_t b;
    };
    uint8_t data[WS28_LED_SIZE];
} adl_ws2812_color_t;

#ifdef __cplusplus
}
#endif
