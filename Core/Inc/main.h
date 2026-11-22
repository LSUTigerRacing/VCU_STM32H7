/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#ifndef CMSIS
#define CMSIS 1
#include "cmsis_os2.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#endif
uint64_t extract_bits(uint64_t data, uint8_t start_bit, uint8_t size);
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define DigIn_3_Pin GPIO_PIN_2
#define DigIn_3_GPIO_Port GPIOE
#define DigIn_4_Pin GPIO_PIN_3
#define DigIn_4_GPIO_Port GPIOE
#define DigIn_5_Pin GPIO_PIN_4
#define DigIn_5_GPIO_Port GPIOE
#define DigOut_9_Pin GPIO_PIN_13
#define DigOut_9_GPIO_Port GPIOC
#define INLET_TEMPERATURE_Pin GPIO_PIN_1
#define INLET_TEMPERATURE_GPIO_Port GPIOC
#define APPS2_Pin GPIO_PIN_2
#define APPS2_GPIO_Port GPIOC
#define OUTLET_TEMPERATURE_Pin GPIO_PIN_2
#define OUTLET_TEMPERATURE_GPIO_Port GPIOA
#define FRONT_BRAKE_PRESSURE_Pin GPIO_PIN_3
#define FRONT_BRAKE_PRESSURE_GPIO_Port GPIOA
#define BACK_BRAKE_PRESSURE_Pin GPIO_PIN_5
#define BACK_BRAKE_PRESSURE_GPIO_Port GPIOA
#define SUSPENSION_FL_Pin GPIO_PIN_6
#define SUSPENSION_FL_GPIO_Port GPIOA
#define SUSPENSION_BR_Pin GPIO_PIN_7
#define SUSPENSION_BR_GPIO_Port GPIOA
#define SUSPENSION_FR_Pin GPIO_PIN_4
#define SUSPENSION_FR_GPIO_Port GPIOC
#define STEERING_ANGLE_Pin GPIO_PIN_5
#define STEERING_ANGLE_GPIO_Port GPIOC
#define THROTTLE_PEDAL_POSITION_Pin GPIO_PIN_0
#define THROTTLE_PEDAL_POSITION_GPIO_Port GPIOB
#define SUSPENSION_BL_Pin GPIO_PIN_1
#define SUSPENSION_BL_GPIO_Port GPIOB
#define FR_WS_Pin GPIO_PIN_14
#define FR_WS_GPIO_Port GPIOB
#define FL_WS_Pin GPIO_PIN_15
#define FL_WS_GPIO_Port GPIOB
#define DigOut_7_Pin GPIO_PIN_10
#define DigOut_7_GPIO_Port GPIOD
#define DigOut_6_Pin GPIO_PIN_11
#define DigOut_6_GPIO_Port GPIOD
#define DigOut_5_Pin GPIO_PIN_15
#define DigOut_5_GPIO_Port GPIOD
#define DigOut_4_Pin GPIO_PIN_6
#define DigOut_4_GPIO_Port GPIOC
#define DigOut_3_Pin GPIO_PIN_8
#define DigOut_3_GPIO_Port GPIOA
#define DigOut_2_Pin GPIO_PIN_9
#define DigOut_2_GPIO_Port GPIOA
#define DigOut_1_Pin GPIO_PIN_10
#define DigOut_1_GPIO_Port GPIOA
#define DigIn_12_Pin GPIO_PIN_0
#define DigIn_12_GPIO_Port GPIOD
#define DigIn_10_Pin GPIO_PIN_1
#define DigIn_10_GPIO_Port GPIOD
#define DigIn_8_Pin GPIO_PIN_7
#define DigIn_8_GPIO_Port GPIOD
#define DigIn_7_Pin GPIO_PIN_5
#define DigIn_7_GPIO_Port GPIOB
#define FL_WSB6_Pin GPIO_PIN_6
#define FL_WSB6_GPIO_Port GPIOB
#define FR_WSB7_Pin GPIO_PIN_7
#define FR_WSB7_GPIO_Port GPIOB
#define RL_WS_Pin GPIO_PIN_8
#define RL_WS_GPIO_Port GPIOB
#define RR_WS_Pin GPIO_PIN_9
#define RR_WS_GPIO_Port GPIOB
#define DigIn_1_Pin GPIO_PIN_0
#define DigIn_1_GPIO_Port GPIOE
#define DigIn_2_Pin GPIO_PIN_1
#define DigIn_2_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */
#define BIT0 (1ULL << 0)
#define BIT1 (1ULL << 1)
#define BIT2 (1ULL << 2)
#define BIT3 (1ULL << 3)
#define BIT4 (1ULL << 4)
#define BIT5 (1ULL << 5)
#define BIT6 (1ULL << 6)
#define BIT7 (1ULL << 7)

#define D1_RAM __attribute__((section(".D1_RAM")))
#define D2_RAM __attribute__((section(".D2_RAM")))
#define D3_RAM __attribute__((section(".D3_RAM")))
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
