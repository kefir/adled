#include "adled.h"
#include "adl_gamma_lut.h"
#include <stdio.h>
#include <string.h>

static uint8_t DUMMY = 0x00;

ADLConfig_t* ledConfig = NULL;

static float max(float a, float b, float c);
static float min(float a, float b, float c);

int32_t adlInit(ADLConfig_t* config)
{
    ADLError_e err = ADL_FAIL;

    if (config) {
        ledConfig = config;

        if (ledConfig->spiDriver && ledConfig->buffer) {
            switch (ledConfig->type) {
            case ADL_WS2812B:
                // ledConfig->spiDriver->write(DUMMY, 1);
                wsBufferInit(ledConfig->buffer, ledConfig->count);
                adlUpdate();
                err = ADL_OK;
                break;
            default:
                err = ADL_WRONG_TYPE;
                break;
            }
        }
    }

    return err;
}

int32_t adlLedSet(uint16_t ledId, ADLRgbColor_t color)
{
    ADLError_e err = ADL_FAIL;
    WSLed_t wsLed;
    if (ledConfig) {
        switch (ledConfig->type) {
        case ADL_WS2812B:
            // wsLed.r = gammaR[color.r];
            // wsLed.g = gammaG[color.g];
            // wsLed.b = gammaB[color.b];
            wsLed.r = color.r;
            wsLed.g = color.g;
            wsLed.b = color.b;
            wsLedSet(ledId, &wsLed);
            err = ADL_OK;
            break;
        default:
            err = ADL_WRONG_TYPE;
            break;
        }
    } else {
        err = ADL_NOINIT;
    }
    return err;
}

int32_t adlFill(ADLRgbColor_t color)
{
    ADLError_e err = ADL_FAIL;
    WSLed_t wsLed;
    if (ledConfig) {
        switch (ledConfig->type) {
        case ADL_WS2812B:
            // wsLed.r = gammaR[color.r];
            // wsLed.g = gammaG[color.g];
            // wsLed.b = gammaB[color.b];
            wsLed.r = color.r;
            wsLed.g = color.g;
            wsLed.b = color.b;
            for (uint16_t i = 0; i < ledConfig->count; i++) {
                wsLedSet(i, &wsLed);
            }
            err = ADL_OK;
            break;
        default:
            err = ADL_WRONG_TYPE;
            break;
        }
    } else {
        err = ADL_NOINIT;
    }
    return err;
}

int32_t adlDrawImage(uint8_t* imageData, int32_t size)
{
    ADLError_e err = ADL_FAIL;
    uint8_t ledCount = 0;
    WSLed_t wsLed;
    if (ledConfig) {
        switch (ledConfig->type) {
        case ADL_WS2812B:
            for (int i = 0; i < size; i += 4) {
                wsLed.b = gammaB[imageData[i] / 2];
                wsLed.g = gammaG[imageData[i + 1] / 2];
                wsLed.r = gammaR[imageData[i + 2] / 2];
                wsLedSet(ledCount++, &wsLed);
            }
            err = ADL_OK;
            break;
        default:
            err = ADL_WRONG_TYPE;
            break;
        }
    } else {
        err = ADL_NOINIT;
    }
    return err;
}

int32_t adlClear()
{
    ADLError_e err = ADL_FAIL;
    if (ledConfig) {
        wsBufferClear(ledConfig->buffer, ledConfig->count);
    } else {
        err = ADL_NOINIT;
    }
    return err;
}

int32_t adlUpdate()
{
    ADLError_e err = ADL_FAIL;
    if (ledConfig) {
        switch (ledConfig->type) {
        case ADL_WS2812B:
            ledConfig->spiDriver->write(&DUMMY, sizeof(uint8_t));
            // ledConfig->spiDriver->mosiLevelSet(0);
            // ledConfig->spiDriver->delayMs(1);
            ledConfig->spiDriver->write(ledConfig->buffer, ledConfig->count * WS28_LED_BUFF_SIZE);
            ledConfig->spiDriver->write(&DUMMY, sizeof(uint8_t));
            // ledConfig->spiDriver->mosiLevelSet(0);
            err = ADL_OK;
            break;
        default:
            err = ADL_WRONG_TYPE;
            break;
        }
    } else {
        err = ADL_NOINIT;
    }
    return err;
}

void adlHexToColor(const char* hexColor, ADLRgbColor_t* color)
{
    if (hexColor[0] == '#') {
        sscanf(hexColor, "#%02x%02x%02x", (unsigned int*)&color->r, (unsigned int*)&color->g, (unsigned int*)&color->b);
    } else {
        sscanf(hexColor, "%02x%02x%02x", (unsigned int*)&color->r, (unsigned int*)&color->g, (unsigned int*)&color->b);
    }
}

void adlRgbToHsv(ADLRgbColor_t* rgbColor, ADLHsvColor_t* hsvColor)
{
    float rd = (float)rgbColor->r / 255;
    float gd = (float)rgbColor->g / 255;
    float bd = (float)rgbColor->b / 255;
    float fMax = max(rd, gd, bd);
    float fMin = min(rd, gd, bd);
    float h = 0;
    float s = 0;
    float v = fMax;

    double d = fMax - fMin;
    s = fMax == 0 ? 0 : d / fMax;

    if (fMax == fMin) {
        h = 0; // achromatic
    } else {
        if (fMax == rd) {
            h = (gd - bd) / d + (gd < bd ? 6 : 0);
        } else if (fMax == gd) {
            h = (bd - rd) / d + 2;
        } else if (fMax == bd) {
            h = (rd - gd) / d + 4;
        }
        h /= 6;
    }

    hsvColor->h = h;
    hsvColor->s = s;
    hsvColor->v = v;
}

void adlHsvToRgb(ADLHsvColor_t* hsvColor, ADLRgbColor_t* rgbColor)
{
    float r = 0, g = 0, b = 0;
    int i = (int)(hsvColor->h * 6);
    float f = hsvColor->h * 6 - i;
    float p = hsvColor->v * (1 - hsvColor->s);
    float q = hsvColor->v * (1 - f * hsvColor->s);
    float t = hsvColor->v * (1 - (1 - f) * hsvColor->s);

    switch (i % 6) {
    case 0:
        r = hsvColor->v, g = t, b = p;
        break;
    case 1:
        r = q, g = hsvColor->v, b = p;
        break;
    case 2:
        r = p, g = hsvColor->v, b = t;
        break;
    case 3:
        r = p, g = q, b = hsvColor->v;
        break;
    case 4:
        r = t, g = p, b = hsvColor->v;
        break;
    case 5:
        r = hsvColor->v, g = p, b = q;
        break;
    }

    rgbColor->r = r * 255;
    rgbColor->g = g * 255;
    rgbColor->b = b * 255;
}

static float max(float a, float b, float c)
{
    return ((a > b) ? (a > c ? a : c) : (b > c ? b : c));
}
static float min(float a, float b, float c)
{
    return ((a < b) ? (a < c ? a : c) : (b < c ? b : c));
}