#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Number of AD Led default palette colors.
 *
 */
#define ADL_PALETTE_COLOR_COUNT 5

/**
 * @brief AD Led default palette colors. RGB (web) format.
 *
 */
static const uint32_t ADL_PALETTE_DEFAULT[ADL_PALETTE_COLOR_COUNT] = {
    0x70a9a1, 0x33658a, 0xcfd7c7, 0xa26769, 0x351e29
};

#ifdef __cplusplus
}
#endif