#include "ws2812.h"
#include "tim.h"
#include "dma.h"
#include <string.h>

uint8_t leds_color_data[LED_CFG_BYTES_PER_LED * LED_CFG_COUNT];
uint32_t dma_buffer[DMA_BUFF_ELE_LEN];
volatile uint8_t is_updating = 0;
volatile uint32_t led_cycles_cnt = 0;

static void led_fill_led_pwm_data(size_t ledx, uint32_t* ptr)
{
    if (ledx < LED_CFG_COUNT) {
        uint32_t r = leds_color_data[ledx * LED_CFG_BYTES_PER_LED + 0];
        uint32_t g = leds_color_data[ledx * LED_CFG_BYTES_PER_LED + 1];
        uint32_t b = leds_color_data[ledx * LED_CFG_BYTES_PER_LED + 2];
        uint32_t arr = htim3.Init.Period + 1;
        uint32_t pulse_high = (3 * arr / 4) - 1;
        uint32_t pulse_low = (1 * arr / 4) - 1;

        for (size_t i = 0; i < 8; i++) {
            ptr[i] = (g & (1 << (7 - i))) ? pulse_high : pulse_low;
            ptr[8 + i] = (r & (1 << (7 - i))) ? pulse_high : pulse_low;
            ptr[16 + i] = (b & (1 << (7 - i))) ? pulse_high : pulse_low;
        }
    }
}

static void led_update_sequence(uint8_t tc)
{
    tc = tc ? 1 : 0;
    if (!is_updating) return;

    led_cycles_cnt += LED_CFG_LEDS_PER_DMA_IRQ;

    if (led_cycles_cnt < LED_RESET_PRE_MIN_LED_CYCLES) {
    } else if (led_cycles_cnt < (LED_RESET_PRE_MIN_LED_CYCLES + LED_CFG_COUNT)) {
        uint32_t next_led = led_cycles_cnt - LED_RESET_PRE_MIN_LED_CYCLES;
        led_fill_led_pwm_data(next_led, &dma_buffer[tc * DMA_BUFF_ELE_HALF_LEN]);
    } else if (led_cycles_cnt < (LED_RESET_PRE_MIN_LED_CYCLES + LED_CFG_COUNT + LED_RESET_POST_MIN_CYCLES + LED_CFG_LEDS_PER_DMA_IRQ)) {
        memset(&dma_buffer[tc * DMA_BUFF_ELE_HALF_LEN], 0, DMA_BUFF_ELE_HALF_SIZEOF);
    } else {
        HAL_DMA_Abort(&hdma_tim3_ch2);
        HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_2);
        is_updating = 0;
    }
}

static uint8_t led_start_transfer(void)
{
    if (is_updating) return 0;

    is_updating = 1;
    led_cycles_cnt = LED_CFG_LEDS_PER_DMA_IRQ;

    memset(dma_buffer, 0, sizeof(dma_buffer));
    for (uint32_t i = 0, index = LED_RESET_PRE_MIN_LED_CYCLES; index < 2 * LED_CFG_LEDS_PER_DMA_IRQ; ++index) {
        led_fill_led_pwm_data(i, &dma_buffer[index * DMA_BUFF_ELE_LED_LEN]);
        i++;
    }

    HAL_DMA_DeInit(&hdma_tim3_ch2);
    if (HAL_DMA_Init(&hdma_tim3_ch2) != HAL_OK) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);
        return 0;
    }

    if (htim3.Instance == NULL) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);
        return 0;
    }

    htim3.Instance->DIER |= TIM_DIER_CC2DE;
    if (HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_2, dma_buffer, DMA_BUFF_ELE_LEN) != HAL_OK) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);
        return 0;
    }

    return 1;
}

void ws2812_init(void)
{
    MX_TIM3_Init();
    MX_DMA_Init();
    MX_GPIO_Init();
    memset(leds_color_data, 0, sizeof(leds_color_data));
}

void set_led_color_hex(uint16_t led_index, uint32_t hex_color, uint8_t brightness)
{
    if (led_index >= LED_CFG_COUNT) return;

    uint8_t r = (hex_color >> 16) & 0xFF;
    uint8_t g = (hex_color >> 8) & 0xFF;
    uint8_t b = hex_color & 0xFF;

    r = (r * brightness) / 100;
    g = (g * brightness) / 100;
    b = (b * brightness) / 100;

    leds_color_data[led_index * LED_CFG_BYTES_PER_LED + 0] = r;
    leds_color_data[led_index * LED_CFG_BYTES_PER_LED + 1] = g;
    leds_color_data[led_index * LED_CFG_BYTES_PER_LED + 2] = b;
}

void set_led_condition_hex(uint8_t condition, uint16_t true_index, uint32_t true_hex, uint8_t true_brightness,
                           uint16_t false_index, uint32_t false_hex, uint8_t false_brightness)
{
    if (condition) {
        set_led_color_hex(true_index, true_hex, true_brightness);
    } else {
        set_led_color_hex(false_index, false_hex, false_brightness);
    }
}

void update_leds(void)
{
    led_start_transfer();
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM3 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2) {
        led_update_sequence(1);
    }
}

void HAL_TIM_PWM_PulseFinishedHalfCpltCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM3 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2) {
        led_update_sequence(0);
    }
}
