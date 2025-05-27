#ifndef __WS2812_H__
#define __WS2812_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

#define LED_CFG_COUNT 12
#define LED_CFG_BYTES_PER_LED 3
#define LED_CFG_LEDS_PER_DMA_IRQ 1
#define DMA_BUFF_ELE_HALF_LEN (LED_CFG_LEDS_PER_DMA_IRQ * LED_CFG_BYTES_PER_LED * 8)
#define DMA_BUFF_ELE_LEN (2 * DMA_BUFF_ELE_HALF_LEN)
#define DMA_BUFF_ELE_HALF_SIZEOF (DMA_BUFF_ELE_HALF_LEN * sizeof(uint32_t))
#define DMA_BUFF_ELE_LED_LEN (LED_CFG_BYTES_PER_LED * 8)
#define LED_RESET_PRE_MIN_LED_CYCLES 2
#define LED_RESET_POST_MIN_CYCLES 2

typedef struct {
    uint8_t Red;
    uint8_t Green;
    uint8_t Blue;
} RGB;

extern uint8_t leds_color_data[LED_CFG_BYTES_PER_LED * LED_CFG_COUNT];
extern volatile uint8_t is_updating;

#define COLOR_PURPLE 0x800080
#define COLOR_GREEN  0x00FF00
#define COLOR_PINK   0xFF1493
#define COLOR_BLUE   0x0000FF
#define COLOR_OFF    0x000000

void ws2812_init(void);
void set_led_color_hex(uint16_t led_index, uint32_t hex_color, uint8_t brightness);
void set_led_condition_hex(uint8_t condition, uint16_t true_index, uint32_t true_hex, uint8_t true_brightness,
                           uint16_t false_index, uint32_t false_hex, uint8_t false_brightness);
void update_leds(void);

#ifdef __cplusplus
}
#endif

#endif
