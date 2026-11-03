/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */
extern osThreadId_t GPIOTaskHandle;
/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, DigOut_5_Pin|DigIn_10_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(DigOut_4_GPIO_Port, DigOut_4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, DigOut_3_Pin|DigOut_2_Pin|DigOut_1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : PE14 */
  GPIO_InitStruct.Pin = GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : SD_Detect_Pin DigIn_12_Pin DigIn_8_Pin */
  GPIO_InitStruct.Pin = SD_Detect_Pin|DigIn_12_Pin|DigIn_8_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : DigOut_5_Pin DigIn_10_Pin */
  GPIO_InitStruct.Pin = DigOut_5_Pin|DigIn_10_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : DigOut_4_Pin */
  GPIO_InitStruct.Pin = DigOut_4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DigOut_4_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : DigOut_3_Pin DigOut_2_Pin DigOut_1_Pin */
  GPIO_InitStruct.Pin = DigOut_3_Pin|DigOut_2_Pin|DigOut_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : DigIn_7_Pin DigIn_5_Pin DigIn_4_Pin DigIn_3_Pin */
  GPIO_InitStruct.Pin = DigIn_7_Pin|DigIn_5_Pin|DigIn_4_Pin|DigIn_3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : DigIn_2_Pin DigIn_1_Pin */
  GPIO_InitStruct.Pin = DigIn_2_Pin|DigIn_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */
//volatile uint32_t button_state = 0;

extern EventGroupHandle_t VCUGroupEventHandle;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_PIN)
{
//Can adjust later, this is to limit timestamp conflict
static uint32_t last_time_6 = 0;
static uint32_t last_time_9 = 0;
static uint32_t last_time_10 = 0;
uint32_t now = xTaskGetTickCountFromISR() * portTICK_PERIOD_MS;
BaseType_t xHigherPriorityTaskWoken = pdFALSE; //Tells RTOS if waking a task requires immediate context switch

switch(GPIO_PIN){
  case GPIO_PIN_6:
  {
    if(now - last_time_6 < DEBOUNCE_MS) break;
    last_time_6 = now;
  if (HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_6) == GPIO_PIN_SET){
  //This replaces the button_state
  xEventGroupSetBitsFromISR(VCUGroupEventHandle, BTN_POWER_MODE,&xHigherPriorityTaskWoken);
  }
  else if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6) == GPIO_PIN_SET){
xEventGroupSetBitsFromISR(VCUGroupEventHandle, BTN_HV_CHECK,&xHigherPriorityTaskWoken);
}
  break;
  }
  case GPIO_PIN_9:
  {
  if(now - last_time_9 < DEBOUNCE_MS) break;
  last_time_9 = now;

    xEventGroupSetBitsFromISR(VCUGroupEventHandle, BTN_RTD, &xHigherPriorityTaskWoken);
  break;
  }

  case GPIO_PIN_10:
  {
  if(now - last_time_10 < DEBOUNCE_MS) break;
  last_time_10 = now;

  xEventGroupSetBitsFromISR(VCUGroupEventHandle, BTN_WATER_PUMP, &xHigherPriorityTaskWoken);
  break;
  }

  default:
      break;
}

portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
/* USER CODE END 2 */
