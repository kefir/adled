#include "adl_ws2812.h"

#include <string.h>

static void ledToBitCode(void* buffPtr, WSLed_t* led);

static void* ledBuffer = NULL;

int32_t wsBufferInit(void* buffer, int32_t len)
{
    ledBuffer = buffer;
    if (ledBuffer) {
        wsBufferClear(ledBuffer, len);
    }
    return 0;
}

void wsBufferClear(void* buffer, int32_t len)
{
    memset(ledBuffer, 0xCC, len * WS28_LED_BUFF_SIZE);
}

void wsLedSet(uint16_t ledIndex, WSLed_t* led)
{
    uint32_t ptr = ledIndex * WS28_LED_BUFF_SIZE;
    ledToBitCode((ledBuffer + ptr), led);
}

static void ledToBitCode(void* buffPtr, WSLed_t* led)
{
    uint8_t buff[WS28_LED_BUFF_SIZE];
    memset(buff, 0xCC, WS28_LED_BUFF_SIZE);
    uint8_t colors[WS28_LED_SIZE] = { led->g, led->r, led->b };
    for (int i = 0; i < WS28_LED_SIZE; i++) {
        int count = 0;
        uint8_t color = colors[i];
        for (int j = 0; j < 8; j += 2) {
            uint8_t value = 0x00;
            value = (color & (1 << j)) ? WS28_ONE_L : WS28_ZERO_L;
            value |= (color & (1 << (j + 1))) ? WS28_ONE_H : WS28_ZERO_H;
            int c = ((i + 1) * WS28_BUFF_MULT) - count - 1;
            buff[c] = value;
            count++;
        }
    }
    memcpy(buffPtr, buff, WS28_LED_BUFF_SIZE);
}