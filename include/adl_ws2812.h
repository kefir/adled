#pragma once

#include <stddef.h>
#include <stdint.h>

#define WS28_ZERO_L 0xC
#define WS28_ONE_L 0xE
#define WS28_ZERO_H 0xC0
#define WS28_ONE_H 0xE0

#define WS28_BUFF_MULT 4
#define WS28_LED_SIZE 3
#define WS28_LED_BUFF_SIZE (WS28_LED_SIZE * WS28_BUFF_MULT)

typedef struct {
    uint8_t g;
    uint8_t r;
    uint8_t b;
} WSLed_t;

int32_t wsBufferInit(void* buffer, int32_t len);
void wsBufferClear(void* buffer, int32_t len);
void wsLedSet(uint16_t ledIndex, WSLed_t* led);