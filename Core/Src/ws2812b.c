/*
 * ws2812b.c
 * Minimalistische Anpassung für STM32G0 + SK6812/WS2812 LEDs
 * TIM3, Channel 2 (PA7) mit DMA1_Channel2
 */

#include "ws2812b.h"

#define BYTE            8
#define BITS_IN_PIXEL   24
#define TIME_TO_RST     50
#define LEDS_COUNT      ((PIXELS_COUNT * BITS_IN_PIXEL) + TIME_TO_RST)
#define LOGIC_ZERO      (26 - 1)
#define LOGIC_ONE       (65 - 1)

static uint8_t ws2812b_buffer[LEDS_COUNT];
static TIM_HandleTypeDef *ws2812b_htim = NULL;
static TIM_TypeDef *ws2812b_timer = NULL;

static void set_bit(uint8_t *buffer, uint8_t byte)
{
    for (uint8_t i = 0; i < BYTE; ++i) {
        if (byte & 0x80)
            buffer[i] = LOGIC_ONE;
        else
            buffer[i] = LOGIC_ZERO;
        byte <<= 1;
    }
}

void Ws2812b_Init(TIM_HandleTypeDef *htim, TIM_TypeDef *TIMx)
{
    ws2812b_htim = htim;
    ws2812b_timer = TIMx;
    for (uint16_t i = 0; i < LEDS_COUNT; ++i) {
        if (i < TIME_TO_RST)
            ws2812b_buffer[i] = 0;
        else
            ws2812b_buffer[i] = LOGIC_ZERO;
    }
}

void Ws2812b_SetPixel_RGB(const RGB *color, uint16_t pixel)
{
    if (pixel >= PIXELS_COUNT) return;
    uint16_t idx = TIME_TO_RST + pixel * BITS_IN_PIXEL;
    set_bit(&ws2812b_buffer[idx + 0], color->Green);
    set_bit(&ws2812b_buffer[idx + 8], color->Red);
    set_bit(&ws2812b_buffer[idx + 16], color->Blue);
}

void Ws2812b_SetStrip_RGB(const RGB *color)
{
    for (uint16_t i = 0; i < PIXELS_COUNT; ++i) {
        Ws2812b_SetPixel_RGB(color, i);
    }
}

void Ws2812b_Show_without_Delay(void)
{
    if (ws2812b_htim)
        HAL_TIM_PWM_Start_DMA(ws2812b_htim, TIM_CHANNEL_2, (uint32_t*)ws2812b_buffer, LEDS_COUNT);
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
    if (ws2812b_htim && htim->Instance == ws2812b_timer)
        HAL_TIM_PWM_Stop_DMA(ws2812b_htim, TIM_CHANNEL_2);
}
