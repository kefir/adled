#pragma once

#include <stddef.h>
#include <stdint.h>

#include "adl_ws2812.h"

typedef enum {
    ADL_OK,
    ADL_NOINIT,
    ADL_WRONG_TYPE,
    ADL_FAIL
} ADLError_e;

typedef enum {
    ADL_DIR_STRAIGHT,
    ADL_ZIGZAG
} ADLDirection_e;

typedef enum {
    ADL_WS2812B,
    APA101
} ADLDiodeType_e;

typedef struct {
    int32_t (*write)(uint8_t* data, uint32_t len);
    void (*mosiLevelSet)(uint8_t level);
    void (*delayMs)(uint32_t delay);
} ADLSpiDriver_t;

typedef struct {
    ADLDirection_e direction;
    ADLDiodeType_e type;
    uint16_t count;
    ADLSpiDriver_t* spiDriver;
    void* buffer;
} ADLConfig_t;

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} ADLRgbColor_t;

typedef struct {
    float h;
    float s;
    float v;
} ADLHsvColor_t;

int32_t adlInit(ADLConfig_t* config);
int32_t adlLedSet(uint16_t ledId, ADLRgbColor_t color);
int32_t adlFill(ADLRgbColor_t color);
int32_t adlDrawImage(uint8_t* imageData, int32_t size);
int32_t adlClear();
int32_t adlUpdate();

void adlHexToColor(const char* hexColor, ADLRgbColor_t* color);
void adlRgbToHsv(ADLRgbColor_t* rgbColor, ADLHsvColor_t* hsvColor);
void adlHsvToRgb(ADLHsvColor_t* hsvColor, ADLRgbColor_t* rgbColor);