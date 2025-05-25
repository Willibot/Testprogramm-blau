/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    dma.c
  * @brief   This file provides code for the configuration
  *          of all the requested memory to memory DMA transfers.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "dma.h"
#include "tim.h"

extern DMA_HandleTypeDef hdma_tim3_ch2;
extern TIM_HandleTypeDef htim3;

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure DMA                                                              */
/*----------------------------------------------------------------------------*/

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/**
  * Enable DMA controller clock
  */
void MX_DMA_Init(void)
{
  __HAL_RCC_DMA1_CLK_ENABLE();

  hdma_tim3_ch2.Instance = DMA1_Channel1;
  hdma_tim3_ch2.Init.Request = DMA_REQUEST_TIM3_CH2;
  hdma_tim3_ch2.Init.Direction = DMA_MEMORY_TO_PERIPH;
  hdma_tim3_ch2.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_tim3_ch2.Init.MemInc = DMA_MINC_ENABLE;
  hdma_tim3_ch2.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdma_tim3_ch2.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
  hdma_tim3_ch2.Init.Mode = DMA_CIRCULAR;
  hdma_tim3_ch2.Init.Priority = DMA_PRIORITY_LOW;
  if (HAL_DMA_Init(&hdma_tim3_ch2) != HAL_OK)
  {
    Error_Handler();
  }

  __HAL_LINKDMA(&htim3, hdma[TIM_DMA_ID_CC2], hdma_tim3_ch2);

  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */

