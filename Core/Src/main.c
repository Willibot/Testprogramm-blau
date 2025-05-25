#include "main.h"
#include "ws2812b.h"

const RGB COLOR_PINK = {255, 20, 147};
const RGB COLOR_OFF  = {0, 0, 0};

int main(void)
{
    HAL_Init();
    SystemClock_Config();

    MX_GPIO_Init();
    MX_DMA_Init();
    MX_TIM3_Init();

    Ws2812b_Init(&htim3, TIM3);

    while (1)
    {
        for (uint16_t pos = 0; pos < PIXELS_COUNT; ++pos)
        {
            // Alle aus
            for (uint16_t i = 0; i < PIXELS_COUNT; ++i)
                Ws2812b_SetPixel_RGB(&COLOR_OFF, i);

            // Eine LED pink
            Ws2812b_SetPixel_RGB(&COLOR_PINK, pos);

            // Senden und warten
            Ws2812b_Show_without_Delay();
            HAL_Delay(60);
        }
    }
}
