/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32g0xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is provided AS-IS and may be freely used as a template.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32g0xx_it.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_tim3_ch2;
extern volatile uint32_t systick_ms;
extern volatile uint8_t ws2812_dma_done;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M0+ Processor Interruption and Exception Handlers          */
/******************************************************************************/

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 0 */
  systick_ms++;
  static uint8_t tick = 0;
  if (++tick >= 2) { // 2 ms bei 1 kHz SysTick, ergibt 500 Hz
      HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8);
      tick = 0;
  }
  /* USER CODE END SysTick_IRQn 0 */
  /* USER CODE BEGIN SysTick_IRQn 1 */
  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32G0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32g0xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 channel 1 interrupt.
  */
void DMA1_Channel1_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_tim3_ch2);
  /* USER CODE BEGIN DMA1_Channel1_IRQn 1 */
  ws2812_dma_done = 1;
  /* USER CODE END DMA1_Channel1_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
